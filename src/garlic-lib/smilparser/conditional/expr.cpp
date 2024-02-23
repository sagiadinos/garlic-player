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

    return execute(MyAdapiWrapper.replaceAdapiFunctions(expr));

}


#if QT_VERSION == 0x060000
bool Expr::execute(QString conv_expr)
{
    try
    {
        XQilla xqilla;
        AutoDelete<XQQuery> query(xqilla.parse(X(conv_expr.toUtf8())));

        AutoDelete<DynamicContext> context(query->createDynamicContext());
        Result result = query->execute(context);

        Item::Ptr item = result->next(context);
        QString bo = UTF8(item->asString(context));

        if (bo.toLower() == "true")
            return true;

    }
    catch(const XQException& e)
    {
        std::cerr << "XQuery Error: " << UTF8(e.getError()) << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown error in XQilla occured." << std::endl;
    }

    return false;

}
#else
bool Expr::execute(QString converted_expr)
{
    QXmlQuery query;
    query.setQuery(converted_expr);
    if (!query.isValid())
        return false;

    query.evaluateTo(&result);

    if (result != "true\n" && result != "true")
        return false;

    return true;
}
#endif
