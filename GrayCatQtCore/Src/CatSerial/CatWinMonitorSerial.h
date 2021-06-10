#ifndef CATWINMONITORSERIAL_H
#define CATWINMONITORSERIAL_H

#include <QObject>

#include <QMutex>

class CatWinMonitorSerial : public QObject
{
    Q_OBJECT
public:
    static CatWinMonitorSerial* Instance() noexcept
    {
        if(_instance == nullptr)
        {
            m_pMutex->lock();
            if(_instance == nullptr)
            {
                _instance = new CatWinMonitorSerial;
            }
            m_pMutex->unlock();
        }
        return _instance;
    }

    static void Delete( void ) noexcept
    {
        if(_instance != nullptr)
        {
            if(m_pMutex != nullptr)
            {
                delete m_pMutex;
                m_pMutex = nullptr;
            }
            delete _instance;
            _instance = nullptr;
        }
    }

private:
    CatWinMonitorSerial();
    ~CatWinMonitorSerial();

signals:
    // 端口插入
    void AddSerial(qint64 pid, qint64 vid);
    // 端口拔出
    void DeleteSerial(qint64 pid, qint64 vid);

private:
    static CatWinMonitorSerial* _instance;
    static QMutex* m_pMutex;

};

#endif // CATWINMONITORSERIAL_H
