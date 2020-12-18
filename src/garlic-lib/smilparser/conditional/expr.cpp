#include "expr.h"

Expr::Expr()
{
}

void Expr::setExpression(QString e)
{
    expr = e;
}

bool Expr::executeQuery()
{
    if (expr.isEmpty()) // play media if expr is empty
        return true;
    QString conv_expr = MyAdapiWrapper.replaceAdapiFunctions(expr);


    query.setQuery(conv_expr);
    if (!query.isValid())
        return false;

    query.evaluateTo(&result);

    if (result != "true\n" && result != "true")
        return false;

    return true;
}

