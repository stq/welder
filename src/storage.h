#pragma once


class Storage {
    static int position;
public:
    static void from(int initialPosition);

    static void read(ulong& value);

    static void write(ulong value);
};