// 想把调用者的类型和返回值分离出去，但模板不能放进类成员函数里（虚函数）

#include <vector>
#include <utility>

using namespace std;

template<class T, class R, typename ... Args>
class MyDelegate {
 public:
    MyDelegate(T* t, R (T::*f)(Args...))
            : m_t(t),
              m_f(f) {
    }
    R Fire(Args ... args) {
        return (m_t->*m_f)(std::forward<Args>(args) ...);
    }
    bool operator==(const MyDelegate &o) {
        return m_t == o.m_t && m_f == o.m_f;
    }
 private:
    T* m_t;
    R (T::*m_f)(Args...);
};

template<class T, class R, typename ... Args>
MyDelegate<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args...)) {
    return MyDelegate<T, R, Args...>(t, f);
}

template<typename ... Args>
class Event {
public:
    template<class T, class R>
    virtual void operator +=(MyDelegate<T, R, Args...> p) = 0;
    virtual void fire(Args ... args) = 0;
    virtual ~Event() = 0;
};

// 底层委派控制
template<class T, class R, typename ... Args>
class DownloadEvent : public Event<Args...> {
 public:
    ~DownloadEvent() {
    }

    void operator +=(MyDelegate<T, R, Args...> p) {
        m_arDelegates.push_back(p);
    }

    void operator -=(MyDelegate<T, R, Args...> p) {
        ITR itr = m_arDelegates.begin();
        while (itr != m_arDelegates.end())
            itr++;
        m_arDelegates.erase(itr);
    }

    void fire(Args ... args) {
        ITR itr = m_arDelegates.begin();
        while (itr != m_arDelegates.end()) {
            itr->Fire(std::forward<Args>(args) ...);
            itr++;
        }
    }

 private:
    vector<MyDelegate<T, R, Args...>> m_arDelegates;
    typedef typename vector<MyDelegate<T, R, Args...>>::iterator ITR;
};

// 中层业务逻辑
class Downloader {
 public:
    void DownloadFile(const char* pURL) {
        cout << "downloading: " << pURL << "" << endl;
        downloadEvent->fire(pURL, true);
    }

    Event<const char*, bool> *downloadEvent;
};

// 顶层业务控制
class CMyFileEx {
 public:
    void download() {
        Downloader downloader;
        downloader.downloadEvent = (Event<const char*, bool> *)new DownloadEvent<CMyFileEx, void, const char*, bool>();
        *downloader.downloadEvent += CreateDelegate(
                this, &CMyFileEx::OnDownloadFinished);
        downloader.DownloadFile("www.baidu.com");
    }

    virtual void OnDownloadFinished(const char* pURL, bool bOK) {
        cout << "OnDownloadFinished, URL:" << pURL << "    status:" << bOK
             << endl;
    }

    virtual ~CMyFileEx() {
    }
};
