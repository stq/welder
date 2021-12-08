#pragma once


class Storage {
    static int position;
public:
    static void from(int initialPosition);

    static void read(unsigned long& value);
    static void read(int& value);

    static void write(unsigned long value);
    static void write(int value);
};