#include "system_info.h"

RestApi::V2::SystemInfo::SystemInfo(QObject *parent) : BaseController{parent}
{

}

QString RestApi::V2::SystemInfo::responseFirmwareInfo()
{
    JsonResponse.reset();
    JsonResponse.insertStringValuePair("firmwareVersion", MyLibfacade->getConfiguration()->getVersion());
    JsonResponse.insertStringValuePair("family", MyLibfacade->getConfiguration()->getAppName() + "-" + MyLibfacade->getConfiguration()->getOS());
    return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::SystemInfo::responseModelInfo()
{
    JsonResponse.reset();
    JsonResponse.insertStringValuePair("modelDescription", "");
    JsonResponse.insertStringValuePair("modelName", MyLibfacade->getConfiguration()->getAppName());
    JsonResponse.insertStringValuePair("modelURL", "");
    JsonResponse.insertStringValuePair("manufacturer", "Sagiadinos");
    JsonResponse.insertStringValuePair("licenseModel", MyLibfacade->getConfiguration()->getOS());
    JsonResponse.insertStringValuePair("PCBRevision", "");
    JsonResponse.insertStringValuePair("manufacturerURL", "https://garlic-player.com");
    JsonResponse.insertStringValuePair("PCB", MyLibfacade->getConfiguration()->getAppName());
    JsonResponse.insertStringValuePair("options", "");
    return JsonResponse.asString(false).toUtf8();
}
