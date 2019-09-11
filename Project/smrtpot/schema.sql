CREATE TABLE IF NOT EXISTS wateringSchedules (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scheduleName TEXT UNIQUE NOT NULL,
    moistureThreshold DECIMAL NOT NULL,
    thresholdEnabled BIT NOT NULL,
    rating INT CHECK (rating > -1 AND rating < 11),
    runningAverage DECIMAL
);

CREATE TABLE IF NOT EXISTS dailyBlocks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    startTime TIME NOT NULL,
    durationSeconds INTEGER NOT NULL,
    wateringSchedule TEXT NOT NULL,
    dayOfWeek INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS weeklyBlocks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    startTime DATETIME NOT NULL,
    durationSeconds INTEGER NOT NULL,
    wateringSchedule INTEGER NOT NULL,
    FOREIGN KEY(wateringSchedule) REFERENCES wateringSchedules(id)
);

CREATE TABLE IF NOT EXISTS plants (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    plantName TEXT UNIQUE NOT NULL,
    startTime DATETIME NOT NULL,
    isCactus BIT NOT NULL DEFAULT 0,
    wateringSchedule INTEGER NOT NULL,
    FOREIGN KEY(wateringSchedule) REFERENCES wateringSchedules(id)
);

/*
  Initialize 2 default watering schedules
  This is necessary as every plant must have a corresponding watering schedule assigned
  It is important that the order (non-cactus, followed by cactus) stays this way
 */
INSERT INTO wateringSchedules (scheduleName, moistureThreshold, thresholdEnabled, rating, runningAverage) VALUES("Non-Cactus default", 0.3, 1, 0, NULL);
INSERT INTO wateringSchedules (scheduleName, moistureThreshold, thresholdEnabled, rating, runningAverage) VALUES("Cactus default", 0.01, 1, 0, NULL);

INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 0, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 0, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 0, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 1, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 1, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 1, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 2, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 2, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 2, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 3, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 3, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 3, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 4, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 4, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 4, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 5, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 5, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 5, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 6, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("13:00:00", 20, 6, "Non-Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("20:00:00", 20, 6, "Non-Cactus default");

INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 0, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 1, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 2, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 3, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 4, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 5, "Cactus default");
INSERT INTO dailyBlocks (startTime, durationSeconds, dayOfWeek, wateringSchedule) VALUES("07:00:00", 20, 6, "Cactus default");
