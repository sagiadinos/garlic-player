#ifndef HTTPD_H
#define HTTPD_H

#include "lib_facade.h"
#include <QCoreApplication>
#include "httplistener.h"
#include "request_mapper.h"
#include "files/free_disc_space.h"


using namespace stefanfrings;

namespace RestApi
{
    class Httpd : public QObject
    {
            Q_OBJECT
        public:
            explicit Httpd(LibFacade *lf, QObject *parent = nullptr);
            void init(QCoreApplication *app);
        private:
            LibFacade          *MyLibFacade;
    };
}
#endif // HTTPD_H
