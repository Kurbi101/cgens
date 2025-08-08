#ifndef PAIR_H
#define PAIR_H

#define DEFINE_PAIR(_name, _type1, _type2)                                     \
	typedef struct _name _name;                                                \
                                                                               \
	struct _name {                                                             \
		_type1 first;                                                          \
		_type2 second;                                                         \
	};

#endif