#include "domain/RetentionPolicy.h"

void RetentionPolicy::changeStorageSize(int v) { maxStorageSize = v; }
void RetentionPolicy::changeMaxDays(int v) { maxDays = v; }
void RetentionPolicy::changeMinRestorePoints(int v) { minRestorePoints = v; }

int RetentionPolicy::getMaxStorageSize() const { return maxStorageSize; }
int RetentionPolicy::getMaxDays() const { return maxDays; }
int RetentionPolicy::getMinRestorePoints() const { return minRestorePoints; }
