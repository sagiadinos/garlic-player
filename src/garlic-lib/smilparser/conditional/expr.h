#ifndef EXPR_H
#define EXPR_H
#include <QString>
#include <QtGlobal>

#if QT_VERSION == 0x060000
    #include <xqilla/xqilla-simple.hpp>
    #include <iostream>
#else
    #include <QXmlQuery>
#endif


#include <QDebug>
#include "adapi_wrapper.h"

class Expr
{
    public:
        Expr();
        void setExpression(QString e);
        bool executeQuery();
    protected:
    private:
        AdapiWrapper MyAdapiWrapper;
        QString expr = "";
        QString result = "";

        bool execute(QString converted_expr);
};

#endif // EXPR_H
