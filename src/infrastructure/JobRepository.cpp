#include "infrastructure/JobRepository.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <filesystem>

void JobRepository::save(const std::vector<BackupJob>& jobs) {
    QJsonArray array;

    for (const auto& job : jobs) {
        QJsonObject obj;

        obj["id"] = QString::fromStdString(job.getId());
        obj["name"] = QString::fromStdString(job.getName());
        obj["source"] = QString::fromStdString(job.getSource().string());
        obj["target"] = QString::fromStdString(job.getTarget().string());
        obj["interval"] = job.getSchedule().getInterval();
        obj["enabled"] = job.isEnabled();
        obj["maxSize"] = job.getRetention().getMaxStorageSize();
        obj["maxDays"] = job.getRetention().getMaxDays();

        auto lastRun = job.getSchedule().getLastRun();
        auto lastRunTime = std::chrono::system_clock::to_time_t(lastRun);
        obj["lastRun"] = static_cast<qint64>(lastRunTime);

        QJsonArray pointsArray;
        for (const auto& point : job.getRestorePoints()) {
            QJsonObject pointObj;
            pointObj["id"] = QString::fromStdString(point.getId());
            pointObj["path"] = QString::fromStdString(point.getPath().string());
            auto timestamp = std::chrono::system_clock::to_time_t(point.getTimestamp());
            pointObj["timestamp"] = static_cast<qint64>(timestamp);
            pointObj["type"] = static_cast<int>(point.getType());
            pointsArray.append(pointObj);
        }
        obj["restorePoints"] = pointsArray;

        QJsonObject hashesObj;
        for (const auto& [filePath, hash] : job.getFileHashes()) {
            hashesObj[QString::fromStdString(filePath)] = QString::fromStdString(hash);
        }
        obj["fileHashes"] = hashesObj;

        array.append(obj);
    }

    QFile file(QString::fromStdString(filePath));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(array).toJson());
        qDebug() << "Saved" << jobs.size() << "jobs to" << QString::fromStdString(filePath);
    } else {
        qDebug() << "ERROR: Failed to save" << QString::fromStdString(filePath);
    }
}

std::vector<BackupJob> JobRepository::load() {
    std::vector<BackupJob> jobs;

    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No" << QString::fromStdString(filePath) << "found, starting fresh";
        return jobs;
    }

    auto doc = QJsonDocument::fromJson(file.readAll());
    auto array = doc.array();

    qDebug() << "Loading" << array.size() << "jobs from" << QString::fromStdString(filePath);

    for (auto val : array) {
        auto obj = val.toObject();

        qint64 lastRunTime = obj["lastRun"].toInteger();
        auto lastRun = std::chrono::system_clock::from_time_t(static_cast<time_t>(lastRunTime));

        Schedule schedule(obj["interval"].toInt(), obj["enabled"].toBool());
        schedule.updateLastRun(lastRun);

        RetentionPolicy retention;
        retention.changeStorageSize(obj["maxSize"].toInt());
        retention.changeMaxDays(obj["maxDays"].toInt());

        BackupJob job(
            obj["id"].toString().toStdString(),
            obj["name"].toString().toStdString(),
            obj["source"].toString().toStdString(),
            obj["target"].toString().toStdString(),
            schedule,
            retention
            );

        if (obj.contains("restorePoints")) {
            auto pointsArray = obj["restorePoints"].toArray();
            for (auto pointVal : pointsArray) {
                auto pointObj = pointVal.toObject();
                std::filesystem::path pointPath = pointObj["path"].toString().toStdString();
                if (std::filesystem::exists(pointPath)) {
                    auto timestamp = std::chrono::system_clock::from_time_t(
                        static_cast<time_t>(pointObj["timestamp"].toInteger())
                        );
                    RestorePoint point(
                        pointObj["id"].toString().toStdString(),
                        pointPath,
                        timestamp,
                        static_cast<BackupType>(pointObj["type"].toInt())
                        );
                    job.addRestorePoint(point);
                } else {
                    qDebug() << "Warning: Restore point not found:" << QString::fromStdString(pointPath.string());
                }
            }
        }

        if (obj.contains("fileHashes")) {
            auto hashesObj = obj["fileHashes"].toObject();
            std::unordered_map<std::string, std::string> restoredHashes;
            for (auto it = hashesObj.begin(); it != hashesObj.end(); ++it) {
                restoredHashes[it.key().toStdString()] = it.value().toString().toStdString();
            }
            job.setFileHashes(restoredHashes);
            qDebug() << "Restored" << restoredHashes.size() << "file hashes for job" << QString::fromStdString(job.getId());
        }

        jobs.push_back(job);
    }

    return jobs;
}