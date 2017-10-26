// 想修改为任意参数，就引入了模板，结果覆盖了太多地方

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

// 底层委派控制
template<class T, class R, typename ... Args>
class Event {
 public:
    ~Event() {
    }

    void operator +=(MyDelegate<T, R, Args...> p) {
        m_arDelegates.push_back(p);
    }

    void operator -=(MyDelegate<T, R, Args...> p) {
        ITR itr = m_arDelegates.begin();
        while (itr != m_arDelegates.end())
            if (*itr++ == p)
                break;
        m_arDelegates.erase(itr);
    }

    void operator()(Args ... args) {
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
template<class T, class R>
class Downloader {
 public:
    R DownloadFile(const char* pURL) {
        cout << "downloading: " << pURL << "" << endl;
        return downloadEvent(pURL, true);
    }

    Event<T, R, const char*, bool> downloadEvent;
};

// 顶层业务控制
class CMyFileEx {
 public:
    void download() {
        Downloader<CMyFileEx, void> downloader;
        downloader.downloadEvent += CreateDelegate(
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
