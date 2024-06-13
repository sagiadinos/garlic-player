#ifndef DERIVED_BASE_H
#define DERIVED_BASE_H

#include "smilparser/base.hpp"

class DerivedBase : public TBase
{
    Q_OBJECT
public:
    explicit DerivedBase(TBase * parent = 0);
    QString   getType();
    void      preloadParse(QDomElement element); // virtual
    void      test_setBaseAttributes();
    void      setActiveElement(QDomElement element);

    QString   getBaseType(){return  "test base";}   // virtual
};


#endif // DERIVED_BASE_H
