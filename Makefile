THEFT_REPO="https://github.com/silentbicycle/theft.git"

THEFT_DIR = dep/theft
THEFT_INCLUDE_DIR = ${THEFT_DIR}/inc
THEFT_LIB = ${THEFT_DIR}/build/libtheft.a

SRC_DIR = src
BUILD_DIR = out
TEST_DIR = test

CINCLUDE += -I${THEFT_INCLUDE_DIR} -I${SRC_DIR} -I${TEST_DIR}
CPPFLAGS +=
LDFLAGS += -lm -lmpfr
CFLAGS += -O2 -g ${CINCLUDE} ${CPPFLAGS} ${LDFLAGS}

OBJECT_NAMES = common fma exact rto mp
OBJECTS = $(addprefix ${BUILD_DIR}/,$(addsuffix .o,${OBJECT_NAMES}))

TEST_NAMES = prop_emu_mp prop_emu_rto
TESTS = $(addprefix ${BUILD_DIR}/,${TEST_NAMES})

all: tests

build: ${BUILD_DIR} ${OBJECTS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -c $< -o $@

tests: build ${TESTS}

${BUILD_DIR}/%: ${TEST_DIR}/%.c ${THEFT_LIB} ${OBJECTS}
	${CC} ${CFLAGS} $< -o $@ ${THEFT_LIB} ${OBJECTS}

${THEFT_LIB}: ${THEFT_DIR}
	${MAKE} -C ${THEFT_DIR}

${THEFT_DIR}:
	mkdir -p dep
	git clone ${THEFT_REPO} ${THEFT_DIR}


.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}

.PHONY: clean-deps
clean-deps:
	${MAKE} -C ${THEFT_DIR} clean
