#pragma once


class Storage {
    static int position;
public:
    static void from(int initialPosition);

    static void read(long &value);

    static void write(long value);
};