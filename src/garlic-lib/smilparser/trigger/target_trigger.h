#ifndef TARGETTRIGGER_H
#define TARGETTRIGGER_H

#include <QObject>
#include <QMap>
/**
 * @brief Comntainer to handle and store the TargetTrigger
 * for an BasTiming Object
 */
class TargetTrigger : public QObject
{
        Q_OBJECT
    public:
        explicit     TargetTrigger(QObject *parent = nullptr);
        explicit     TargetTrigger();
        void         insert(QString trigger, QString target_id);
        QStringList  findTargetIDsByTrigger(QString trigger);
    private:
        QMap<QString, QString> trigger_list = {};
    signals:

};

#endif // TARGETTRIGGER_H
