// �������ö�̬ȥ��ģ��ģ��������˶�̬������Ҫԭ��һ��

//#include <vector>
//#include <utility>
//
//using namespace std;
//
//template<class T, class F>
//class MyDelegate {
// public:
//    MyDelegate(T* t, F f)
//            : m_t(t),
//              m_f(f) {
//    }
//    R Fire(Args... args) {
//        return (m_t->*m_f)(std::forward<Args>(args) ...);
//    }
// private:
//    T* m_t;
//    F m_f;
//};
//
//template<class T, class R, typename ... Args>
//MyDelegate<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args...)) {
//    return MyDelegate<T, R, Args...>(t, f);
//}
//
//// �ײ�ί�ɿ���
//template<class F>
//class CDownloadEvent {
// public:
//    CDownloadEvent() {
//    }
//    ~CDownloadEvent() {
//    }
//
//    void operator +=(F p) {
//        m_arDelegates.push_back(p);
//    }
//
//    void operator -=(F p) {
////        ITR itr = remove(m_arDelegates.begin(), m_arDelegates.end(), p);
////
////        ITR itrTemp = itr;
////        while (itrTemp != m_arDelegates.end())
////        {
////            delete *itr;
////            ++itr;
////        }
////        m_arDelegates.erase(itr, m_arDelegates.end());
//    }
//
//    void operator()(const char* pURL, bool bOK) {
//        ITR itrTemp = m_arDelegates.begin();
//        while (itrTemp != m_arDelegates.end()) {
//            itrTemp->Fire(pURL, bOK);
//            ++itrTemp;
//        }
//    }
//
// private:
//    vector<F> m_arDelegates;
//    typedef typename vector<F>::iterator ITR;
//};
//
//// �в�ҵ���߼�
//class CMyDownloaderEx {
// public:
//    void DownloadFile(const char* pURL) {
//        cout << "downloading: " << pURL << "" << endl;
//        downloadEvent(pURL, true);
//    }
//
//    CDownloadEvent *downloadEvent;
//};
//
//// ����ҵ�����
//class CMyFileEx {
// public:
//    void download() {
//        CMyDownloaderEx downloader;
//        downloader.downloadEvent += CreateDelegate(
//                this, &CMyFileEx::OnDownloadFinished);
//        downloader.DownloadFile("www.baidu.com");
//    }
//
//    virtual void OnDownloadFinished(const char* pURL, bool bOK) {
//        cout << "OnDownloadFinished, URL:" << pURL << "    status:" << bOK
//             << endl;
//    }
//
//    virtual ~CMyFileEx() {
//    }
//};
