#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#include <cstring>

struct Destructor
{
private:
	struct IDestructorFunctor;

	template<class T>
	struct DestructorFunctor;

public:
	Destructor() :
		destructor_(nullptr),
		object_(nullptr)
	{
	}

	template<class T>
	Destructor(T *object)
	{
		attune(object);
	}

	Destructor(const Destructor &other)
	{
		operator=(other);
	}

	const Destructor &operator=(const Destructor &rhs)
	{
		if (this != &rhs)
			std::memcpy(this, &rhs, sizeof(Destructor));

		return *this;
	}

	template<class T>
	void attune()
	{
		destructor_ = new(&dummy_) DestructorFunctor<T>;
	}

	template<class T>
	void attune(void *object)
	{
		attune<T>();
		object_ = object;
	}

	void operator()(void *object)
	{
		(*destructor_)(object);
	}

	void operator()()
	{
		(*destructor_)(object_);
		object_ = nullptr;
	}

	explicit operator bool() const
	{
		return destructor_ != nullptr;
	}

private:
	struct IDestructorFunctor
	{
		virtual void operator()(void *obj) = 0;
	};

	template<class T>
	struct DestructorFunctor : IDestructorFunctor
	{
		virtual void operator()(void *obj) override
		{
			reinterpret_cast<T*>(obj)->~T();
		}
	};

	IDestructorFunctor *destructor_;
	DestructorFunctor<int> dummy_;
	void *object_;
};

#endif // DESTRUCTOR_H