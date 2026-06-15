#ifndef BESTTIMESTORAGE_H
#define BESTTIMESTORAGE_H

class BestTimeStorage {
  public:
    static float Load();
    static void SaveIfBetter(float timeSeconds);
};

#endif // BESTTIMESTORAGE_H