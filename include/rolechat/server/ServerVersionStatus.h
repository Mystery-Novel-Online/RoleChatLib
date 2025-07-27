#ifndef ROLECHAT_SERVER_SERVER_VERSION_STATUS_H
#define ROLECHAT_SERVER_SERVER_VERSION_STATUS_H
#include <string>
namespace rolechat::server {
enum class VersionStatus
{
  VersionOk,
  VersionIncompatible,
  VersionClientOutdated,
  VersionServerOutdated,
};
}
#endif