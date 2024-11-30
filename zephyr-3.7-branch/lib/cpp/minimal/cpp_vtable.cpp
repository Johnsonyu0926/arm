//lib/cpp/minial/cpp_vtable.cpp
namespace __cxxabiv1 {
	class __class_type_info {
		virtual void dummy();
	};
	class __si_class_type_info {
		virtual void dummy();
	};
	void __class_type_info::dummy() { }  // causes the vtable to get created here
	void __si_class_type_info::dummy() { }  // causes the vtable to get created here
};
//GST