#pragma once

// Real sensor data - info for reference
// v               charging               not-charging
//          disconnected connected disconnected connected
// 14.83              161       941         162      1006
// 13.20              147       800         153       954
// 10.50              134       774         106       839
// 05.60              100       693          67       762
// 03.00               85       590          41       580
//Results:
//Contact - > 200
//No contact - < 200

class Sensor {
    static void analyzeReadings();
public:
    static bool isContacted;
    static void tick();
};
