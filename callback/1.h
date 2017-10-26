// 由原版http://www.cnblogs.com/qq78292959/archive/2012/10/10/2719155.html经微调

#ifndef NULL
#define NULL 0
#endif

#include <vector>

using namespace std;

// 回调接口
class IDelegate
{
public:
    virtual void Fire(const char* pURL, bool bOK) = 0;
    virtual ~IDelegate() = 0;
};

// 回调实现
template<typename O, typename T>
class CDownloadDelegate: public IDelegate
{
    typedef void (T::*Fun)(const char*, bool);
public:
    CDownloadDelegate(O* pObj = NULL, Fun pFun = NULL)
        :m_pFun(pFun), m_pObj(pObj)
    {
    }

    virtual void Fire(const char* pURL, bool bOK)
    {
        if(m_pFun != NULL
            && m_pObj != NULL)
        {
            (m_pObj->*m_pFun)(pURL, bOK);
        }
    }

private:
    Fun m_pFun;
    O* m_pObj;
};

template<typename O, typename T>
CDownloadDelegate<O,T>* MakeDelegate(O* pObject, void (T::*pFun)(const char* pURL, bool))
{
    return new CDownloadDelegate<O, T>(pObject, pFun);
}

// 底层委派控制
class CDownloadEvent
{
public:
    ~CDownloadEvent()
    {
        ITR itr = m_arDelegates.begin();
        while (itr != m_arDelegates.end())
        {
            delete *itr;
            ++itr;
        }
        m_arDelegates.clear();
    }

    void operator += (IDelegate* p)
    {
        m_arDelegates.push_back(p);
    }

    void operator -= (IDelegate* p)
    {
        ITR itr = remove(m_arDelegates.begin(), m_arDelegates.end(), p);

        ITR itrTemp = itr;
        while (itrTemp != m_arDelegates.end())
        {
            delete *itr;
            ++itr;
        }
        m_arDelegates.erase(itr, m_arDelegates.end());
    }

    void operator()(const char* pURL, bool bOK)
    {
        ITR itrTemp = m_arDelegates.begin();
        while (itrTemp != m_arDelegates.end())
        {
            (*itrTemp)->Fire(pURL, bOK);
            ++itrTemp;
        }
    }

private:
    vector<IDelegate*> m_arDelegates;
    typedef vector<IDelegate*>::iterator ITR;
};

// 中层业务逻辑
class CMyDownloaderEx
{
public:
    void DownloadFile(const char* pURL)
    {
        cout << "downloading: " << pURL << "" << endl;
        downloadEvent(pURL, true);
    }

    CDownloadEvent downloadEvent;
};

// 顶层业务控制
class CMyFileEx
{
public:
    void download()
    {
        CMyDownloaderEx downloader;
        downloader.downloadEvent += MakeDelegate(this, &CMyFileEx::OnDownloadFinished);
        downloader.DownloadFile("www.baidu.com");
    }

    virtual void OnDownloadFinished(const char* pURL, bool bOK)
    {
        cout << "OnDownloadFinished, URL:" << pURL << "    status:" << bOK << endl;
    }

    virtual ~CMyFileEx() {}
};
