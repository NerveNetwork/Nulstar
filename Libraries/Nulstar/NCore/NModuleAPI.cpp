#include <NMessage.h>
#include "NCoreConstants.h"
#include "NModuleAPI.h"

namespace NulstarNS {
  NModuleAPI::NModuleAPI()
            : mIsValid(false) {

  }

  NModuleAPI::NModuleAPI(const QVariantMap& lModuleApiMap)
            : mIsValid(false), mFindDependenciesRetryCounter(0) {
    fFillFields(lModuleApiMap);
  }

  NModuleAPI::~NModuleAPI() {
    //qDeleteAll(mModuleRoles);
  }

  void NModuleAPI::fFillFields(const QVariantMap& lModuleApiMap) {
    if(!lModuleApiMap.contains(cFieldName_ModuleName))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleName)));
      return;
    }
    mModuleName = lModuleApiMap.value(cFieldName_ModuleName).toString();

    if(!lModuleApiMap.contains(cFieldName_Dependencies))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(mModuleName).arg(cFieldName_Dependencies)));
      return;
    }
    mDependencies = lModuleApiMap.value(cFieldName_Dependencies).toMap();

    if(!lModuleApiMap.value(cFieldName_ConnectionInformation).toMap().contains(cFieldName_IP))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(mModuleName).arg(cFieldName_IP)));
      return;
    }
    mIP = QHostAddress(lModuleApiMap.value(cFieldName_ConnectionInformation).toMap().value(cFieldName_IP).toString());
    if(mIP.isNull()) {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(mModuleName).arg(cFieldName_IP)));
      return;
    }

    if(!lModuleApiMap.contains(cFieldName_Methods))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(mModuleName).arg(cFieldName_Methods)));
      return;
    }
    mMethods = lModuleApiMap.value(cFieldName_Methods).toList();

    if(!lModuleApiMap.contains(cFieldName_Abbreviation))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_Abbreviation)));
      return;
    }
    mModuleAbbreviation = lModuleApiMap.value(cFieldName_Abbreviation).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleDomain))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleDomain)));
      return;
    }
    mModuleDomain = lModuleApiMap.value(cFieldName_ModuleDomain).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleRoles))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(mModuleName).arg(cFieldName_ModuleRoles)));
      return;
    }
    QVariantMap lRoleMap = lModuleApiMap.value(cFieldName_ModuleRoles).toMap();
    QMapIterator<QString, QVariant> i1(lRoleMap);
    while(i1.hasNext()) {
      i1.next();
      NModuleAPIRole lAPIRole;// = new NModuleAPIRole(i1.key(), QVersionNumber::fromString(i1.value().toString()));
      QStringList lVersionsSupported;
      for(const QVariant& lVersionSupported : i1.value().toList())
        lVersionsSupported << lVersionSupported.toString();
      lAPIRole.fSetRoleName(i1.key());
      lAPIRole.fSetVersionsSupported(lVersionsSupported);
      mModuleRoles << lAPIRole;
    }

    if(!lModuleApiMap.contains(cFieldName_ModuleVersion))  {
      qDebug("%s", qUtf8Printable(QString("API from '%1' module received! (Field '%2' does not contain valid information)").arg(mModuleName).arg(cFieldName_ModuleVersion)));
      return;
    }
    mModuleVersion = lModuleApiMap.value(cFieldName_ModuleVersion).toString();

    if(!lModuleApiMap.value(cFieldName_ConnectionInformation).toMap().contains(cFieldName_Port)) {
      qDebug("%s", qUtf8Printable(QString("API from '%1' module received! (Field '%2' does not contain valid information)").arg(mModuleName).arg(cFieldName_Port)));
      return;
    }
    mPort = lModuleApiMap.value(cFieldName_ConnectionInformation).toMap().value(cFieldName_Port).toString().toUShort();
    mIsValid = true;
  }

  bool NModuleAPI::fIsMethodSupported(const QString& lMethodName) const {
    for(const QVariant& lMethod : mMethods) {
       if((lMethod.toMap().value(cFieldName_MethodName).toString() == lMethodName))
          return true;
    }
    return false;
  }

  bool NModuleAPI::fIsRoleSupported(const NModuleAPIRole& lModuleRole) const {
    for(const NModuleAPIRole& lModuleAPIRole : mModuleRoles) {
      if((lModuleRole.fVersionsSupported().size()) && (lModuleRole.fRoleName() == lModuleAPIRole.fRoleName()) && (lModuleAPIRole.fVersionsSupported().contains(lModuleRole.fVersionsSupported().at(0))))
        return true;
    }
    return false;
  }

  bool NModuleAPI::fAreRolesNeeded(const QList<NModuleAPIRole>& lModuleRoles) const {
    for(const NModuleAPIRole& lModuleRole : lModuleRoles) {
      if(mDependencies.contains(lModuleRole.fRoleName()) && (lModuleRole.fVersionsSupported().contains(mDependencies.value(lModuleRole.fRoleName()).toString())))
        return true;
    }
    return false;
  }
}
