#ifndef HTTPD_H
#define HTTPD_H

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
            explicit Httpd(MainConfiguration *mc, DB::InventoryTable *it, FreeDiscSpace *fds, QObject *parent = nullptr);
            void init(QCoreApplication *app);
        private:
            MainConfiguration  *MyConfiguration;
            DB::InventoryTable *MyInventoryTable;
            FreeDiscSpace      *MyFreeDiscSpace;
    };
}
#endif // HTTPD_H
