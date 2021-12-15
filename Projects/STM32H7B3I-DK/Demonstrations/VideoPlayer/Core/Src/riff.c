// take care: whenever we call rh->fp_read() or rh->fp_seek()
//   we must adjust rh->c_pos and rh->pos
//   => to simplify user wrappers we update the positions outside


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h> //function with variable number of arguments

#include "riff.h"


#define RIFF_LEVEL_ALLOC 16  //number of stack elements allocated per step lock more when needing to enlarge (step)

void stack_push(riff_handle *rh, char *type);
int riff_printf(const char *format, ... );
size_t read_file(void *fh, void *ptr, size_t size);
size_t seek_file(void *fh, size_t pos);
size_t read_mem(void *fh, void *ptr, size_t size);
int riff_open_file(riff_handle *rh, FIL *f, size_t size);
unsigned int readUInt32LE(riff_handle *rh);
size_t seek_mem(void *fh, size_t pos);
unsigned int convUInt32LE(void *p);
void stack_pop(riff_handle *rh);
int riff_readChunkHeader(riff_handle *rh);


//table to translate Error code to string
//shall correspond to RIFF_ERROR_... macros
static const char *riff_es[] = {
	//0
	"No error",
	//1
	"End of chunk",
	//2
	"End of chunk list",
	//3
	"Excess bytes at end of file",

	//4
	"Illegal four character id",
	//5
	"Chunk size exceeds list level or file",
	//6
	"End of RIFF file",
	//7
	"File access failed",
	//8
	"Invalid riff_handle",


	//9
	//all other
	"Unknown RIFF error"
};




//*** default access FP setup ***

/*****************************************************************************/
//default print function
int riff_printf(const char *format, ... ){
#if 0
	va_list args;
	va_start(args, format);
	int r = printf(format, args);
	va_end (args);

        return r;
#endif
	return -1;
}


//** FIL **


/*****************************************************************************/
size_t read_file(void *fh, void *ptr, size_t size){
  size_t br;
  f_read((FIL*)fh, ptr, size, &br);

  return br;
}

/*****************************************************************************/
size_t seek_file(void *fh, size_t pos){
	f_lseek((FIL*)fh, pos);
	return pos;
}

/*****************************************************************************/
//description: see header file
int riff_open_file(riff_handle *rh, FIL *f, size_t size){
	if(rh == NULL)
		return RIFF_ERROR_INVALID_HANDLE;
	if(rh != NULL){
		rh->fh = f;
		rh->size = size;
		rh->pos_start = f_tell(f); //current file offset of stream considered as start of RIFF file

		rh->fp_read = &read_file;
		rh->fp_seek = &seek_file;

		riff_readHeader(rh);
	}
	return RIFF_ERROR_NONE;
}



//** memory **


/*****************************************************************************/
size_t read_mem(void *fh, void *ptr, size_t size){
	memcpy(ptr, fh, size);
	return size;
}

/*****************************************************************************/
size_t seek_mem(void *fh, size_t pos){
	return pos; //instant in memory
}

/*****************************************************************************/
//description: see header file
int riff_open_mem(riff_handle *rh, void *ptr, size_t size){
	if(rh == NULL)
		return RIFF_ERROR_INVALID_HANDLE;

	rh->fh = ptr;
	rh->size = size;
	//rh->pos_start = 0 //redundant -> passed memory pointer is always expected to point to start of riff file

	rh->fp_read = &read_mem;
	rh->fp_seek = &seek_mem;

	riff_readHeader(rh);

	return RIFF_ERROR_NONE;
}



// **** internal ****



/*****************************************************************************/
//pass pointer to 32 bit LE value and convert, return in native byte order
unsigned int convUInt32LE(void *p){
	unsigned char *c = (unsigned char*)p;
	return c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);
}


/*****************************************************************************/
//read 32 bit LE from file via FP and return as native
unsigned int readUInt32LE(riff_handle *rh){
	char buf[4] = {0};
	rh->fp_read(rh->fh, buf, 4);
	rh->pos += 4;
	rh->c_pos += 4;
	return convUInt32LE(buf);
}


/*****************************************************************************/
//read chunk header
//return error code
int riff_readChunkHeader(riff_handle *rh){
	char buf[8];

	int n = rh->fp_read(rh->fh, buf, 8);

	if(n != 8){
		if(rh->fp_printf)
			rh->fp_printf("Failed to read header, %d of %d bytes read!\n", n, 8);
		return RIFF_ERROR_EOF; //return error code
	}

	rh->c_pos_start = rh->pos;
	rh->pos += n;

	memcpy(rh->c_id, buf, 4);
	rh->c_size = convUInt32LE(buf + 4);
	rh->pad = rh->c_size & 0x1; //pad byte present if size is odd
	rh->c_pos = 0;


	//verify valid chunk ID, must contain only printable ASCII chars
	int i;
	for(i = 0; i < 4; i++) {
		if(rh->c_id[i] < 0x20  ||  rh->c_id[i] > 0x7e) {
			if(rh->fp_printf)
				rh->fp_printf("Invalid chunk ID (FOURCC) of chunk at file pos %d: 0x%02x,0x%02x,0x%02x,0x%02x\n", rh->c_pos_start, rh->c_id[0], rh->c_id[1], rh->c_id[2], rh->c_id[3]);
			return RIFF_ERROR_ILLID;
		}
	}


	//check if chunk fits into current list level and file, value could be corrupt
	size_t cposend = rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET + rh->c_size + rh->pad;

	size_t listend;
	if(rh->ls_level > 0){
		struct riff_levelStackE *ls = rh->ls + (rh->ls_level - 1);
		listend = ls->c_pos_start + RIFF_CHUNK_DATA_OFFSET + ls->c_size; //end of current list level without pad byte
	}
	else
		listend = rh->pos_start + RIFF_CHUNK_DATA_OFFSET + rh->h_size;

	if(cposend > listend){
		if(rh->fp_printf)
			rh->fp_printf("Chunk size exceeds list size! At least one size value must be corrupt!");
		//chunk data must be considered as cut off, better skip this chunk
		return RIFF_ERROR_ICSIZE;
	}

	//check chunk size against file size
	if((rh->size > 0)  &&  (cposend > rh->size)){
		if(rh->fp_printf)
			rh->fp_printf("Chunk size exceeds file size! At least one size value must be corrupt!");
		return RIFF_ERROR_EOF; //Or better RIFF_ERROR_ICSIZE?
	}

	return RIFF_ERROR_NONE;
}


/*****************************************************************************/
//pop from level stack
//when returning we are positioned inside the parent chunk ()
void stack_pop(riff_handle *rh){
	if(rh->ls_level <= 0)
		return;

	rh->ls_level--;
	struct riff_levelStackE *ls = rh->ls + rh->ls_level;

	rh->c_pos_start = ls->c_pos_start;
	memcpy(rh->c_id, ls->c_id, 4);
	rh->c_size = ls->c_size;
	rh->pad = rh->c_size & 0x1; //pad if chunk sizesize is odd

	rh->c_pos = rh->pos - rh->c_pos_start - RIFF_CHUNK_DATA_OFFSET;
}


/*****************************************************************************/
//push to level stack
void stack_push(riff_handle *rh, char *type){
	//need to enlarge stack?
	if(rh->ls_size < rh->ls_level + 1){
		size_t ls_size_new = rh->ls_size * 2; //double size
		if(ls_size_new == 0)
			ls_size_new = RIFF_LEVEL_ALLOC; //default stack allocation

		struct riff_levelStackE *lsnew = malloc(ls_size_new * sizeof(struct riff_levelStackE));
		rh->ls_size = ls_size_new;

		//need to copy?
		if(rh->ls_level > 0){
			memcpy(lsnew, rh->ls, rh->ls_level * sizeof(struct riff_levelStackE));
		}

		//free old
		if(rh->ls != NULL)
			free(rh->ls);
		rh->ls = lsnew;
	}

	struct riff_levelStackE *ls = rh->ls + rh->ls_level;
	ls->c_pos_start = rh->c_pos_start;
	strcpy((char *)ls->c_id, (char *)rh->c_id);
	ls->c_size = rh->c_size;
	//printf("list size %d\n", (rh->ls[rh->ls_level].size));
	strcpy((char *)ls->c_type, (char *)type);
	rh->ls_level++;
}


//**** user access ****


/*****************************************************************************/
//description: see header file
riff_handle *riff_handleAllocate(void){
	riff_handle *rh = calloc(1, sizeof(riff_handle));
	if(rh != NULL){
		rh->fp_printf = riff_printf;
	}
	return rh;
}

/*****************************************************************************/
//description: see header file
//Deallocate riff_handle and contained stack, file source (memory) is not closed or freed
void riff_handleFree(riff_handle *rh){
	if(rh == NULL)
		return;
	//free stack
	if(rh->ls != NULL)
		free(rh->ls);
	//free struct
	free(rh);
}

/*****************************************************************************/
//description: see header file
//shall be called only once by the open-function
int riff_readHeader(riff_handle *rh){
	char buf[RIFF_HEADER_SIZE];

	if(rh->fp_read == NULL) {
		if(rh->fp_printf)
			rh->fp_printf("I/O function pointer not set\n"); //fatal user error
		return RIFF_ERROR_INVALID_HANDLE;
	}

	int n = rh->fp_read(rh->fh, buf, RIFF_HEADER_SIZE);
	rh->pos += n;

	if(n != RIFF_HEADER_SIZE){
		if(rh->fp_printf)
			rh->fp_printf("Read error, failed to read RIFF header\n");
		//printf("%d", n);
		return RIFF_ERROR_EOF; //return error code
	}
	memcpy(rh->h_id, buf, 4);
	rh->h_size = convUInt32LE(buf + 4);
	memcpy(rh->h_type, buf + 8, 4);


	if(strcmp(rh->h_id, "RIFF") != 0) {
		if(rh->fp_printf)
			rh->fp_printf("Invalid RIFF header\n");
		return RIFF_ERROR_ILLID;
	}

	int r = riff_readChunkHeader(rh);
	if(r != RIFF_ERROR_NONE)
		return r;

	//compare with given file size
	if(rh->size != 0){
		if(rh->size != rh->h_size + RIFF_CHUNK_DATA_OFFSET){
			if(rh->fp_printf)
				rh->fp_printf("RIFF header chunk size %d doesn't match file size %d!\n", rh->h_size + RIFF_CHUNK_DATA_OFFSET, rh->size);
			if(rh->size >= rh->h_size + RIFF_CHUNK_DATA_OFFSET)
				return RIFF_ERROR_EXDAT;
			else
				//end isn't reached yet and you can parse further
				//but file seems to be cut off or given file size (via open-function) was too small -> we are not allowed to read beyond
				return RIFF_ERROR_EOF;
		}
	}

	return RIFF_ERROR_NONE;
}



// **** external ****



//make use of user defined functions via FPs

/*****************************************************************************/
//read to memory block, returns number of successfully read bytes
//keep track of position, do not read beyond end of chunk, pad byte is not read
size_t riff_readInChunk(riff_handle *rh, void *to, size_t size){
	size_t left = rh->c_size - rh->c_pos;
	if(left < size)
		size = left;
	size_t n = rh->fp_read(rh->fh, to, size);
	rh->pos += n;
	rh->c_pos += n;
	return n;
}

/*****************************************************************************/
//seek byte position in current chunk data from start of chunk data, return error on failure
//keep track of position
//c_pos: relative offset from chunk data start
int riff_seekInChunk(riff_handle *rh, size_t c_pos){
	//seeking behind last byte is valid, next read at that pos will fail
	if(c_pos > rh->c_size){
		return RIFF_ERROR_EOC;
	}
	rh->pos = rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET + c_pos;
	rh->c_pos = c_pos;
	rh->fp_seek(rh->fh, rh->pos); //seek never fails, but pos might be invalid to read from
	return RIFF_ERROR_NONE;
}


/*****************************************************************************/
//description: see header file
int riff_seekNextChunk(riff_handle *rh){
	size_t posnew = rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET + rh->c_size + rh->pad; //expected pos of following chunk

	size_t listend;
	if(rh->ls_level > 0){
		struct riff_levelStackE *ls = rh->ls + (rh->ls_level - 1);
		listend = ls->c_pos_start + RIFF_CHUNK_DATA_OFFSET + ls->c_size; //end of current list level without pad byte
	}
	else
		listend = rh->pos_start + RIFF_CHUNK_DATA_OFFSET + rh->h_size; //at level 0

	//printf("listend %d  posnew %d\n", listend, posnew);  //debug

	//if no more chunks in the current sub list level
	if(listend < posnew + RIFF_CHUNK_DATA_OFFSET){
		//there shouldn't be any pad bytes at the list end, since the containing chunks should be padded to even number of bytes already
		//we consider excess bytes as non critical file structure error
		if(listend > posnew){
			if(rh->fp_printf)
				rh->fp_printf("%d excess bytes at pos %d at end of chunk list!\n", listend - posnew, posnew);
			return RIFF_ERROR_EXDAT;
		}
		return RIFF_ERROR_EOCL;
	}

	rh->pos = posnew;
	rh->c_pos = 0;
	rh->fp_seek(rh->fh, posnew);

	return riff_readChunkHeader(rh);
}


/*****************************************************************************/
int riff_seekChunkStart(struct riff_handle *rh){
	//seek data offset 0 in current chunk
	rh->pos = rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET;
	rh->c_pos = 0;
	rh->fp_seek(rh->fh, rh->pos);
	return RIFF_ERROR_NONE;
}


/*****************************************************************************/
int riff_rewind(struct riff_handle *rh){
	//pop stack as much as possible
	while(rh->ls_level > 0) {
		stack_pop(rh);
	}
	return riff_seekLevelStart(rh);
}

/*****************************************************************************/
int riff_seekLevelStart(struct riff_handle *rh){
	//if in sub list level
	if(rh->ls_level > 0)
		rh->pos = rh->ls[rh->ls_level - 1].c_pos_start;
	else
		rh->pos = rh->pos_start;

	rh->pos += RIFF_CHUNK_DATA_OFFSET + 4; //pos after type ID of chunk list
	rh->c_pos = 0;
	rh->fp_seek(rh->fh, rh->pos);

	//read first chunk header, so we have the right values
	int r = riff_readChunkHeader(rh);

	//check possible?
	return r;
}


/*****************************************************************************/
//description: see header file
int riff_seekLevelSub(riff_handle *rh){
	//according to "https://en.wikipedia.org/wiki/Resource_Interchange_File_Format" only RIFF and LIST chunk IDs can contain subchunks
	if(strcmp(rh->c_id, "LIST") != 0  &&  strcmp(rh->c_id, "RIFF") != 0){
		if(rh->fp_printf)
			rh->fp_printf("%s() failed for chunk ID \"%s\", only RIFF or LIST chunk can contain subchunks", __func__, rh->c_id);
		return RIFF_ERROR_ILLID;
	}

	//check size of parent chunk data, must be at least 4 for type ID (is empty list allowed?)
	if(rh->c_size < 4){
		if(rh->fp_printf)
			rh->fp_printf("Chunk too small to contain sub level chunks\n");
		return RIFF_ERROR_ICSIZE;
	}

	//seek to chunk start if not there, required to read type ID
	if(rh->c_pos > 0) {
		rh->fp_seek(rh->fh, rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET);
		rh->pos = rh->c_pos_start + RIFF_CHUNK_DATA_OFFSET;
		rh->c_pos = 0;
	}
	//read type ID
	unsigned char type[5] = "\0\0\0\0\0";
	rh->fp_read(rh->fh, type, 4);
	rh->pos += 4;
	//verify type ID
	int i;
	for(i = 0; i < 4; i++) {
		if(type[i] < 0x20  ||  type[i] > 0x7e) {
			if(rh->fp_printf)
				rh->fp_printf("Invalid chunk type ID (FOURCC) of chunk at file pos %d: 0x%02x,0x%02x,0x%02x,0x%02x\n", rh->c_pos_start, type[0], type[1], type[2], type[3]);
			return RIFF_ERROR_ILLID;
		}
	}

	//add parent chunk data to stack
	//push
	stack_push(rh, (char *)type);

	return riff_readChunkHeader(rh);
}


/*****************************************************************************/
//description: see header file
int riff_levelParent(struct riff_handle *rh){
	if(rh->ls_level <= 0)
		return -1;  //not critical error, we don't have or need a macro for that
	stack_pop(rh);
	return RIFF_ERROR_NONE;
}


/*****************************************************************************/
int riff_levelValidate(struct riff_handle *rh){
	int r;
	//seek to start of current list
	if((r = riff_seekLevelStart(rh))  !=  RIFF_ERROR_NONE)
		return r;

	//seek all chunks of current list level
	while(1){
		r = riff_seekNextChunk(rh);
		if(r != RIFF_ERROR_NONE){
			if(r == RIFF_ERROR_EOCL) //just end of list
				break;
			//error occurred, was probably printed already
			return r;
		}
	}
	return RIFF_ERROR_NONE;
}


/*****************************************************************************/
//description: see header file
const char *riff_errorToString(int e){
	//map error to error string
	//Make sure mapping is correct!
	switch (e){
		case RIFF_ERROR_NONE:
			return riff_es[RIFF_ERROR_NONE];
		case RIFF_ERROR_EOC:
			return riff_es[RIFF_ERROR_EOC];
		case RIFF_ERROR_EOCL:
			return riff_es[RIFF_ERROR_EOCL];
		case RIFF_ERROR_EXDAT:
			return riff_es[RIFF_ERROR_EXDAT];
		case RIFF_ERROR_ILLID:
			return riff_es[RIFF_ERROR_ILLID];
		case RIFF_ERROR_ICSIZE:
			return riff_es[RIFF_ERROR_ICSIZE];
		case RIFF_ERROR_EOF:
			return riff_es[RIFF_ERROR_EOF];
		case RIFF_ERROR_ACCESS:
			return riff_es[RIFF_ERROR_ACCESS];
		case RIFF_ERROR_INVALID_HANDLE:
			return riff_es[RIFF_ERROR_INVALID_HANDLE];
		default:
			return  riff_es[9];
	}
}

