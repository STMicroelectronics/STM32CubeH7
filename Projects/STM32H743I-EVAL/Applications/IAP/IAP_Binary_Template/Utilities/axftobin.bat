@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5.23\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM32H743I_EVAL\STM32H743I_EVAL_SysTick.bin ..\MDK-ARM\STM32H743I_EVAL\STM32H743I_EVAL.axf 

echo AXF to BIN file generation completed.

