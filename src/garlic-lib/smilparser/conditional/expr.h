#ifndef EXPR_H
#define EXPR_H
#include <QString>
#include <QXmlQuery>
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
        QXmlQuery query;
        QString expr = "";
        QString result = "";

};

#endif // EXPR_H
