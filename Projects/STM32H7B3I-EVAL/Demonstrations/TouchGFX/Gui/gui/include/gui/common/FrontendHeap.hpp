#ifndef FRONTENDHEAP_HPP
#define FRONTENDHEAP_HPP

#include <gui_generated/common/FrontendHeapBase.hpp>

class FrontendHeap : public FrontendHeapBase
{
public:
    /* List any user-defined view types here*/
    typedef meta::TypeList< meta::Nil, //Replace this with first user-defined type
            meta::Nil  //List must always end with meta::Nil !
            > UserDefinedViewTypes;

    /* List any user-defined presenter types here*/
    typedef meta::TypeList< meta::Nil, //Replace this with first user-defined type
            meta::Nil  //List must always end with meta::Nil !
            > UserDefinedPresenterTypes;

    /* List any user-defined transition types here*/
    typedef meta::TypeList< meta::Nil, //Replace this with first user-defined type
            meta::Nil  //List must always end with meta::Nil !
            > UserDefinedTransitionTypes;

    /* Calculate largest view, both from generated and user-defined typelists */
    typedef meta::select_type_maxsize< UserDefinedViewTypes >::type MaxUserViewType;

    typedef meta::TypeList< MaxGeneratedViewType,
            meta::TypeList< MaxUserViewType,
            meta::Nil
            > > CombinedViewTypes;

    typedef meta::select_type_maxsize< CombinedViewTypes >::type MaxViewType;

    /* Calculate largest presenter, both from generated and user-defined typelists */
    typedef meta::select_type_maxsize< UserDefinedPresenterTypes >::type MaxUserPresenterType;

    typedef meta::TypeList< MaxGeneratedPresenterType,
            meta::TypeList< MaxUserPresenterType,
            meta::Nil
            > > CombinedPresenterTypes;
    typedef meta::select_type_maxsize< CombinedPresenterTypes >::type MaxPresenterType;

    /* Calculate largest transition, both from generated and user-defined typelists */
    typedef meta::select_type_maxsize< UserDefinedTransitionTypes >::type MaxUserTransitionType;

    typedef meta::TypeList< MaxGeneratedTransitionType,
            meta::TypeList< MaxUserTransitionType,
            meta::Nil
            > > CombinedTransitionTypes;
    typedef meta::select_type_maxsize< CombinedTransitionTypes >::type MaxTransitionType;

    static FrontendHeap& getInstance()
    {
        static FrontendHeap instance;
        return instance;
    }

    Partition< CombinedPresenterTypes, 1 > presenters;
    Partition< CombinedViewTypes, 1 > views;
    Partition< CombinedTransitionTypes, 1 > transitions;
    FrontendApplication app;
    Model model;

private:
    FrontendHeap() : FrontendHeapBase(presenters, views, transitions, app),
        app(model, *this)
    {
        gotoStartScreen(app);
    }
};

#endif // FRONTENDHEAP_HPP
