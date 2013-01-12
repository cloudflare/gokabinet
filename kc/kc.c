// Copyright 2013 gokabinet authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <kclangc.h>
#include "kc.h"

KCREC
gokccurget(KCCUR *cur)
{
	KCREC p;
	const char *argvbuf;
	p.key.buf = kccurget(cur, &p.key.size, &argvbuf, &p.value.size, 1);
	p.value.buf = (char *)argvbuf;
	return p;
}

void
_alloc(char ***v, size_t n)
{
	int i;
	*v = (char **)malloc(n * sizeof(char *));
	for(i = 0; i < n; ++i) {
		(*v)[i] = (char *)malloc(MAX_RECORD_SIZE * sizeof(char));
	}
}

void
_free(char ***v, size_t n)
{
	int i;
	for(i = 0; i < n; ++i) {
		free((*v)[i]);
	}
	free(*v);
}

struct strary
match_prefix(KCDB *db, char *prefix, size_t max)
{
	int i;
	int64_t n;
	struct strary s;
	_alloc(&s.v, max);
	n = kcdbmatchprefix(db, prefix, s.v, max);
	if(n == -1) {
		_free(&s.v, max);
		s.v = nil;
		return s;
	}
	s.n = n;
	if(n < max) {
		for(i = n; i < max; ++i) {
			free(s.v[i]);
		}
		s.v = (char **)realloc(s.v, s.n * sizeof(char *));
	}
	return s;
}

char *
strary_item(struct strary *s, int64_t position)
{
	if(position < s->n) {
		return s->v[position];
	}
	return nil;
}

void
free_strary(struct strary *s)
{
	_free(&s->v, s->n);
}
