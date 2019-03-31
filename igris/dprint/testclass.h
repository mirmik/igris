#ifndef GXX_TESTCLASS_H
#define GXX_TESTCLASS_H


class TestClass {

	const char* mnem = nullptr;

public:
	const char* data() const { return "TestClass::data"; }
	size_t size() const { return 15; }

	void print_prefix() {
		dpr("TestClass(");
		if (mnem) dpr(mnem);
		dpr(")::");
	}

	explicit TestClass(const char* mnem = nullptr) : mnem(mnem) { 
		print_prefix();
		dprln("ctor");  
	}

	explicit TestClass(const TestClass& other) : mnem(other.mnem) { 
		print_prefix();
		dprln("copy_ctor");  
	}
	
	explicit TestClass(TestClass&& other) : mnem(other.mnem) { 
		print_prefix();
		dprln("move_ctor");  
	}

	~TestClass() {
		print_prefix();
		dprln("dtor");  
	}

	void method() {
		print_prefix();
		dprln("method");  		
	}

	int method(int i) {
		print_prefix();
		dpr("method->");
		dprln(i);  		
		return i;
	}
};

#endif