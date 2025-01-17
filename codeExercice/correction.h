/** @file readerwritepriowritergeneral.h
 *  @brief Reader-writer with priority for readers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers. This class follows the "general" way of solving
 *  concurrent problems: internal variables, one mutex, and semaphores
 *  as synchronization primitives.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

#ifndef READERWRITERPRIOWRITERGENERAL_H
#define READERWRITERPRIOWRITERGENERAL_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcohoaremonitor.h>

#include "abstractreaderwriter.h"


class ReaderWriterPrioWriterGeneral :
      public AbstractReaderWriter {
protected:
  PcoSemaphore mutex;

  PcoSemaphore readerBlocker;
  int nbReadersWaiting;

  int nbReaders;

  PcoSemaphore writerBlocker;
  int nbWritersWaiting;

  bool oneWriter;

public:
  ReaderWriterPrioWriterGeneral() :
    mutex(1),
    nbReadersWaiting(0),
    nbReaders(0),
    nbWritersWaiting(0),
    oneWriter(false)
    {}

    // redacteur prioritaire donc en attente si redacteur(s) deja en attente
    void lockReading() override {
        mutex.acquire();
        if (oneWriter || (nbWritersWaiting > 0)) {
            nbReadersWaiting++;
            mutex.release();
            readerBlocker.acquire();
        } else {
            nbReaders++;
            mutex.release();
        }
    }

    // redacteur prioritaire donc faire passer redacteur si un en attente et si plus de lecteur(s)
    void unlockReading() override {
        mutex.acquire();
        nbReaders--;
        if ((nbReaders == 0) && (nbWritersWaiting > 0)) {
            // faire rentrer un redacteur
            oneWriter = true;
            nbWritersWaiting--;
            writerBlocker.release();
        }
        mutex.release();
    }

    // redacteur prioritaire donc faire passer redacteur si pas de lecture(s) / ecriture en cours
    void lockWriting() override {
        mutex.acquire();
        if (oneWriter || (nbReaders > 0)) {
            nbWritersWaiting++;
            mutex.release();
            writerBlocker.acquire();
        } else {
            oneWriter = true;
            mutex.release();
        }
    }

    // redacteur prioritaire donc faire passer redacteur si un en attente
    void unlockWriting() override {
        mutex.acquire();
        oneWriter = false;
        if (nbWritersWaiting > 0) {
            oneWriter = true;
            nbWritersWaiting--;
            writerBlocker.release();
        } else if (nbReadersWaiting > 0) {
            for(int i = 0; i < nbReadersWaiting; i++) {
                readerBlocker.release();
            }
            nbReaders = nbReadersWaiting;
            nbReadersWaiting = 0;
        }
        mutex.release();
    }
};
#endif // READERWRITERPRIOWRITERGENERAL_H
