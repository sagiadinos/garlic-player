#include "system_info.h"

RestApi::V2::SystemInfo::SystemInfo(QObject *parent)
    : BaseController{parent}
{

}

QString RestApi::V2::SystemInfo::responseFirmwareInfo()
{
    JsonResponse.reset();
    JsonResponse.insertStringValuePair("firmwareVersion", MyConfiguration->getVersion());
    JsonResponse.insertStringValuePair("family", MyConfiguration->getAppName() + "-" + MyConfiguration->getOS());
    return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::SystemInfo::responseModelInfo()
{
    JsonResponse.reset();
    JsonResponse.insertStringValuePair("modelDescription", "");
    JsonResponse.insertStringValuePair("modelName", MyConfiguration->getAppName());
    JsonResponse.insertStringValuePair("modelURL", "");
    JsonResponse.insertStringValuePair("manufacturer", "Sagiadinos");
    JsonResponse.insertStringValuePair("licenseModel", MyConfiguration->getOS());
    JsonResponse.insertStringValuePair("PCBRevision", "");
    JsonResponse.insertStringValuePair("manufacturerURL", "https://garlic-player.com");
    JsonResponse.insertStringValuePair("PCB", MyConfiguration->getAppName());
    JsonResponse.insertStringValuePair("options", "");
    return JsonResponse.asString(false).toUtf8();
}
