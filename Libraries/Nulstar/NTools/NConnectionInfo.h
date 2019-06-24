#ifndef NCONNECTIONINFO_H
#define NCONNECTIONINFO_H

#include <QObject>
#include <QString>
#include "NTools.h"

namespace NulstarNS {
  class NTOOLSSHARED_EXPORT NConnectionInfo  : public QObject {
    Q_OBJECT

    public:
      NConnectionInfo(const quint8 lReconnectionTryInterval, const QString& lRole, const QString& lIP, const QString& lPort, const QString& lAPIVersion);

      quint8 fGetReconnectionTryInterval() const { return mReconnectionTryInterval; }
      QString fGetRole() const { return mRole; }
      QString fGetIP() const { return mIP; }
      QString fGetPort() const { return mPort; }
      QString fGetAPIVersion() const { return mAPIVersion; }

      bool fIsValid();

    private:
      quint8 mReconnectionTryInterval;
      QString mRole;
      QString mIP;
      QString mPort;
      QString mAPIVersion;
  };
}

#endif // NCONNECTIONINFO_H
