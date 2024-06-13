#include "derived_base.hpp"

DerivedBase::DerivedBase(TBase *parent) : TBase(parent)
{
    setObjectName("testBasePlaylist");
}

QString DerivedBase::getType()
{
    return "test base";
}

void DerivedBase::preloadParse(QDomElement element)
{
    root_element = element;  // Simple implementation for testing purposes
}

void DerivedBase::test_setBaseAttributes()
{
    setBaseAttributes();
}

void DerivedBase::setActiveElement(QDomElement element)
{
    root_element = element;
}
