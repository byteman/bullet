#ifndef SINGLETONHOLDER_H
#define SINGLETONHOLDER_H
#include <QMutex>

template <class S>
class SingletonHolder
    /// This is a helper template class for managing
    /// singleton objects allocated on the heap.
    /// The class ensures proper deletion (including
    /// calling of the destructor) of singleton objects
    /// when the application that created them terminates.
{
public:
    SingletonHolder():
        _pS(0)
        /// Creates the SingletonHolder.
    {
    }

    ~SingletonHolder()
        /// Destroys the SingletonHolder and the singleton
        /// object that it holds.
    {
        delete _pS;
    }

    S* get()
        /// Returns a pointer to the singleton object
        /// hold by the SingletonHolder. The first call
        /// to get will create the singleton.
    {
        _m.lock();
        if (!_pS) _pS = new S;
        _m.unlock();
        return _pS;
    }

    void reset()
        /// Deletes the singleton object.
    {
         _m.lock();
        delete _pS;
        _pS = 0;
        _m.unlock();
    }

private:
    S* _pS;
    QMutex _m;
};




#endif // SINGLETONHOLDER_H

