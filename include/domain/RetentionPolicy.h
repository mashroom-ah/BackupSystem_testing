#pragma once

class RetentionPolicy {
private:
    int maxStorageSize = 0;
    int maxDays = 0;
    int minRestorePoints = 3;

public:
    void changeStorageSize(int v);
    void changeMaxDays(int v);
    void changeMinRestorePoints(int v);

    int getMaxStorageSize() const;
    int getMaxDays() const;
    int getMinRestorePoints() const;
};
