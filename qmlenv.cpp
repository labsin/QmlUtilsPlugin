#include "qmlenv.h"
#include <QProcess>
#include <QProcessEnvironment>
#include <QDir>
#include <QFile>
#include <QDebug>

QmlEnv::QmlEnv(QObject *parent) :
    QObject(parent)
{
}

QmlEnv::~QmlEnv()
{
}

QString QmlEnv::readXdg(const QString type) {
    QProcess proc;
    proc.start( "xdg-user-dir", QStringList() << type );
    if (!proc.waitForStarted() || !proc.waitForFinished())
        return QString();
    return QString::fromLocal8Bit( proc.readAll()).trimmed();
}

QString QmlEnv::readEnvVar(const QString var) {
    QProcessEnvironment pe = QProcessEnvironment::systemEnvironment();
    return pe.value(var);
    QByteArray env = qgetenv(var.toStdString().c_str());
    return QString(env);
}

bool QmlEnv::mkdir(const QString dir) {
    QString tmp_dir = dir;
    tmp_dir.remove(QRegExp("^\\w*://"));
    if(QDir(tmp_dir).exists()) {
        return true;
    }
    return QDir().mkdir(tmp_dir);
}

bool QmlEnv::fileExists(const QString file) {
    QString tmp_file = file;
    tmp_file.remove(QRegExp("^\\w*://"));
    return QFile(tmp_file).exists();
}
