#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton {
public:
	Singleton() { }
	virtual ~Singleton() { }

	static T* getInstance() {
                if (instance)
                    return instance;
                else
                    return 0;
	}

	static void newInstance() {
		if(instance)
			delete instance;

		instance = new T();
	}

	static void deleteInstance() {
		if(instance)
			delete instance;
                instance = 0;
	}
protected:
        static T* volatile instance;
private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);

};

template<class T> T* volatile Singleton<T>::instance = 0;

#endif // SINGLETON_H
