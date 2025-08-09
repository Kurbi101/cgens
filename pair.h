#ifndef PAIR_H
#define PAIR_H

#define DEFINE_PAIR(_name, _type1, _type2)                                     \
	typedef struct _name _name;                                                \
                                                                               \
	struct _name {                                                             \
		_type1 first;                                                          \
		_type2 second;                                                         \
	};

#define DEFINE_NAMED_PAIR(_name, _type1, _type2, _name1, _name2)               \
	typedef struct _name _name;                                                \
                                                                               \
	struct _name {                                                             \
		_type1 _name1;                                                         \
		_type2 _name2;                                                         \
	};

#endif