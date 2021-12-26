#include "tetris.h"

#include <ctime>
#include <sstream>

// namespace TetrisAPI {

Tetris::Block::SRS::SRS(Type type) : test{}, length(-1) {
	static const Coordinate JLSTZ[4][2][5] = {
		{
			// 0 >> 1
			{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
			// 0 >> 3
			{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}
		},
		{
			// 1 >> 2
			{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
			// 1 >> 0
			{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}
		},
		{
			// 2 >> 3
			{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
			// 2 >> 1
			{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}
		},
		{
			// 3 >> 0
			{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
			// 3 >> 2
			{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
		}
	};
	static const Coordinate I[4][2][5] = {
		{
			// 0 >> 1
			{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
			// 0 >> 3
			{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}
		},
		{
			// 1 >> 2
			{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
			// 1 >> 0
			{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}
		},
		{
			// 2 >> 3
			{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
			// 2 >> 1
			{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}
		},
		{
			// 3 >> 0
			{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},
			// 3 >> 2
			{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}
		}
	};
	static const Coordinate O[4][2][1] = {
		{
			// 0 >> 1
			{{0, 0}},
			// 0 >> 3
			{{0, 0}}
		},
		{
			// 1 >> 2
			{{0, 0}},
			// 1 >> 0
			{{0, 0}}
		},
		{
			// 2 >> 3
			{{0, 0}},
			// 2 >> 1
			{{0, 0}}
		},
		{
			// 3 >> 0
			{{0, 0}},
			// 3 >> 2
			{{0, 0}}
		}
	};
	switch (type) {
	case Tetris::Block::Type::J:
	case Tetris::Block::Type::L:
	case Tetris::Block::Type::S:
	case Tetris::Block::Type::T:
	case Tetris::Block::Type::Z:
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 2; j++)
				test[i][j] = JLSTZ[i][j];
		length = 5;
		break;
	case Tetris::Block::Type::I:
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 2; j++)
				test[i][j] = I[i][j];
		length = 5;
		break;
	case Tetris::Block::Type::O:
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 2; j++)
				test[i][j] = O[i][j];
		length = 1;
		break;
	default:
		break;
	}
}

Tetris::Block::Block(Type type, int width, int height, std::vector<std::vector<std::vector<UBYTE>>> data)
	: type(type), width(width), height(height), data{}, srs(type) {
	for (int k = 0; k < 4; k++) {
		this->data[k] = new UBYTE * [height];
		for (int i = 0; i < height; i++) {
			this->data[k][i] = new UBYTE[width];
			for (int j = 0; j < width; j++)
				this->data[k][i][j] = data[k][i][j];
		}
	}
}

Tetris::Block::~Block() {
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < height; i++)
			delete[] data[k][i];
		delete[] data[k];
	}
}

const Tetris::Block Tetris::block[Block::Type::LENGTH] = {
	{
		Tetris::Block::Type::Z,
		3, 3,
		{
			{
				{0x32, 0x3C, 0x00},
				{0x00, 0x33, 0x38},
				{0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x00, 0x34},
				{0x00, 0x36, 0x39},
				{0x00, 0x31, 0x00}
			},
			{
				{0x00, 0x00, 0x00},
				{0x32, 0x3C, 0x00},
				{0x00, 0x33, 0x38}
			},
			{
				{0x00, 0x34, 0x00},
				{0x36, 0x39, 0x00},
				{0x31, 0x00, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::L,
		3, 3,
		{
			{
				{0x00, 0x00, 0x44},
				{0x42, 0x4A, 0x49},
				{0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x44, 0x00},
				{0x00, 0x45, 0x00},
				{0x00, 0x43, 0x48}
			},
			{
				{0x00, 0x00, 0x00},
				{0x46, 0x4A, 0x48},
				{0x41, 0x00, 0x00}
			},
			{
				{0x42, 0x4C, 0x00},
				{0x00, 0x45, 0x00},
				{0x00, 0x41, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::O,
		4, 3,
		{
			{
				{0x00, 0x56, 0x5C, 0x00},
				{0x00, 0x53, 0x59, 0x00},
				{0x00, 0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x56, 0x5C, 0x00},
				{0x00, 0x53, 0x59, 0x00},
				{0x00, 0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x56, 0x5C, 0x00},
				{0x00, 0x53, 0x59, 0x00},
				{0x00, 0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x56, 0x5C, 0x00},
				{0x00, 0x53, 0x59, 0x00},
				{0x00, 0x00, 0x00, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::S,
		3, 3,
		{
			{
				{0x00, 0x66, 0x68},
				{0x62, 0x69, 0x00},
				{0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x64, 0x00},
				{0x00, 0x63, 0x6C},
				{0x00, 0x00, 0x61}
			},
			{
				{0x00, 0x00, 0x00},
				{0x00, 0x66, 0x68},
				{0x62, 0x69, 0x00}
			},
			{
				{0x64, 0x00, 0x00},
				{0x63, 0x6C, 0x00},
				{0x00, 0x61, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::I,
		4, 4,
		{
			{
				{0x00, 0x00, 0x00, 0x00},
				{0x72, 0x7A, 0x7A, 0x78},
				{0x00, 0x00, 0x00, 0x00},
				{0x00, 0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x00, 0x74, 0x00},
				{0x00, 0x00, 0x75, 0x00},
				{0x00, 0x00, 0x75, 0x00},
				{0x00, 0x00, 0x71, 0x00}
			},
			{
				{0x00, 0x00, 0x00, 0x00},
				{0x00, 0x00, 0x00, 0x00},
				{0x72, 0x7A, 0x7A, 0x78},
				{0x00, 0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x74, 0x00, 0x00},
				{0x00, 0x75, 0x00, 0x00},
				{0x00, 0x75, 0x00, 0x00},
				{0x00, 0x71, 0x00, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::J,
		3, 3,
		{
			{
				{0x84, 0x00, 0x00},
				{0x83, 0x8A, 0x88},
				{0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x86, 0x88},
				{0x00, 0x85, 0x00},
				{0x00, 0x81, 0x00}
			},
			{
				{0x00, 0x00, 0x00},
				{0x82, 0x8A, 0x8C},
				{0x00, 0x00, 0x81}
			},
			{
				{0x00, 0x84, 0x00},
				{0x00, 0x85, 0x00},
				{0x82, 0x89, 0x00}
			}
		}
	},
	{
		Tetris::Block::Type::T,
		3, 3,
		{
			{
				{0x00, 0x94, 0x00},
				{0x92, 0x9B, 0x98},
				{0x00, 0x00, 0x00}
			},
			{
				{0x00, 0x94, 0x00},
				{0x00, 0x97, 0x98},
				{0x00, 0x91, 0x00}
			},
			{
				{0x00, 0x00, 0x00},
				{0x92, 0x9E, 0x98},
				{0x00, 0x91, 0x00}
			},
			{
				{0x00, 0x94, 0x00},
				{0x92, 0x9D, 0x00},
				{0x00, 0x91, 0x00}
			}
		}
	}
};

Tetris::Info::Info(int width) :
	width(width), line_height(_line_height), max_height(_max_height), clear_count(_clear_count), total_blocks(_total_blocks) {
	_line_height = new int[width];
	reset();
}
Tetris::Info::Info(const Info& info) :
	width(info.width), line_height(_line_height), max_height(_max_height), clear_count(_clear_count), total_blocks(_total_blocks) {
	_line_height = new int[width];
	for (int i = 0; i < width; i++)
		_line_height[i] = info._line_height[i];
	_max_height = info._max_height;
	_clear_count = info._clear_count;
	_total_blocks = info._total_blocks;
}
Tetris::Info::~Info() {
	delete[] _line_height;
}

void Tetris::Info::reset() {
	for (int i = 0; i < width; i++)
		_line_height[i] = 0;
	_max_height = 0;
	_clear_count = 0;
	_total_blocks = -1;
}

Tetris::Board::Board(int width, int height, int sx, int sy) :
	width(width), height(height), sx(sx), sy(sy),
	data(ref_data), shadow(ref_shadow) {
	auto init = [](UBYTE**& arr, const int& width, const int& height, const int& sx, const int& sy) {
		arr = new UBYTE * [height + sy * 2];
		for (int i = 0; i < height + sy * 2; i++)
			arr[i] = new UBYTE[width + sx * 2]();
	};
	init(_data, width, height, sx, sy);
	init(_shadow, width, height, sx, sy);
	clearingBuf = new UBYTE[height]();
	init(board_buf, width, height, 0, 0);
	clear_buf = new UBYTE[height]();
	// init refs
	auto ref_data_temp = new UBYTE * [height];
	auto ref_shadow_temp = new UBYTE * [height];
	for (int i = 0; i < height; i++) {
		ref_data_temp[i] = _data[i + sy] + sx;
		ref_shadow_temp[i] = _shadow[i + sy] + sx;
	}
	ref_data = ref_data_temp;
	ref_shadow = ref_shadow_temp;
}
Tetris::Board::Board(const Board& board) :
	width(board.width), height(board.height), sx(board.sx), sy(board.sy),
	data(ref_data), shadow(ref_shadow) {
	auto copy_init = [](UBYTE**& dest, UBYTE**const& src, const int& width, const int& height, const int& sx, const int& sy) {
		dest = new UBYTE * [height + sy * 2];
		for (int i = 0; i < height + sy * 2; i++) {
			dest[i] = new UBYTE[width + sx * 2];
			for (int j = 0; j < width + sx * 2; j++)
				dest[i][j] = src[i][j];
		}
	};
	copy_init(_data, board._data, width, height, sx, sy);
	copy_init(_shadow, board._shadow, width, height, sx, sy);
	clearingBuf = new UBYTE[height];
	for (int i = 0; i < height; i++)
		clearingBuf[i] = board.clearingBuf[i];
	copy_init(board_buf, board.board_buf, width, height, 0, 0);
	clear_buf = new UBYTE[height];
	for (int i = 0; i < height; i++)
		clear_buf[i] = board.clear_buf[i];
	// init refs
	auto ref_data_temp = new UBYTE * [height];
	auto ref_shadow_temp = new UBYTE * [height];
	for (int i = 0; i < height; i++) {
		ref_data_temp[i] = _data[i + sy] + sx;
		ref_shadow_temp[i] = _shadow[i + sy] + sx;
	}
	ref_data = ref_data_temp;
	ref_shadow = ref_shadow_temp;
}
Tetris::Board::~Board() {
	auto dele = [&](UBYTE**& arr, const int& height, const int& sy) {
		for (int i = 0; i < height + sy * 2; i++)
			delete[] arr[i];
		delete[] arr;
	};
	dele(_data, height, sy);
	dele(_shadow, height, sy);
	delete[] clearingBuf;
	dele(board_buf, height, 0);
	delete[] clear_buf;
	// delete refs
	delete[] ref_data;
	delete[] ref_shadow;
}

void Tetris::Board::clear() {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			_data[i + sy][j + sx] = 0x00;
	for (int i = 0; i < height; i++) {
		_data[i + sy][sx - 1] = _data[i + sy][width + sx] = 0x15;
	}
	for (int j = 0; j < width; j++) {
		_data[sy - 1][j + sx] = 0x00;
		_data[height + sy][j + sx] = 0x1A;
	}
	_data[sy][sx - 1] = 0x14;
	_data[sy - 1][sx - 1] = 0x14;
	_data[height + sy][sx - 1] = 0x13;
	_data[sy][width + sx] = 0x14;
	_data[sy - 1][width + sx] = 0x14;
	_data[height + sy][width + sx] = 0x19;
	// clear shadow
	for (int i = 0; i < height + 2 * sy; i++)
		for (int j = 0; j < width + 2 * sx; j++)
			_shadow[i][j] = 0x00;
	// clear clearing buf
	for (int i = 0; i < height; i++)
		clearingBuf[i] = 0;
}
void Tetris::Board::pasteBlock(int x, int y, Tetris::Block::Type type, int rot) {
	auto& b = block[type];
	for (int i = 0; i < b.height; i++)
		for (int j = 0; j < b.width; j++) {
			const int cx = sx + x + j, cy = sy + y + i;
			_data[cy][cx] |= b.data[rot][i][j];
			if (y + i >= 0)
				clearingBuf[y + i] += !!b.data[rot][i][j];
		}
}
void Tetris::Board::eraseBlock(int x, int y, Tetris::Block::Type type, int rot) {
	auto& b = block[type];
	for (int i = 0; i < b.height; i++)
		for (int j = 0; j < b.width; j++) {
			const int cx = sx + x + j, cy = sy + y + i;
			_data[cy][cx] -= b.data[rot][i][j];
			if (y + i >= 0)
				clearingBuf[y + i] -= !!b.data[rot][i][j];
		}
}
void Tetris::Board::pasteShadow(int x, int y, Tetris::Block::Type type, int rot) {
	auto& b = block[type];
	for (int i = 0; i < b.height; i++)
		for (int j = 0; j < b.width; j++) {
			const int cx = sx + x + j, cy = sy + y + i;
			_shadow[cy][cx] |= b.data[rot][i][j];
		}
}
void Tetris::Board::eraseShadow(int x, int y, Tetris::Block::Type type, int rot) {
	auto& b = block[type];
	for (int i = 0; i < b.height; i++)
		for (int j = 0; j < b.width; j++) {
			const int cx = sx + x + j, cy = sy + y + i;
			if (b.data[rot][i][j])
				_shadow[cy][cx] = 0x00;
		}
}
int Tetris::Board::testCollision(int x, int y, Tetris::Block::Type type, int rot) {
	auto& b = block[type];
	int collisionTimes = 0;
	for (int i = 0; i < b.height; i++)
		for (int j = 0; j < b.width; j++) {
			const int cx = sx + x + j, cy = sy + y + i;
			collisionTimes += !!(_data[cy][cx] && b.data[rot][i][j]);
		}
	return collisionTimes;
}
void Tetris::Board::eraseLine(int line) {
	const int iline = sy + line;
	for (int i = 0; i < width; i++) {
		_data[iline][sx + i] = 0;
		_data[iline - 1][sx + i] &= 0xFB;
		_data[iline + 1][sx + i] &= 0xFE;
	}
	clearingBuf[line] = 0;
}
void Tetris::Board::shift(int start, int dest, int lines) {
	int buf_len = start + 1 < lines ? start + 1 : lines;
	for (int i = 0; i < buf_len; i++) {
		for (int j = 0; j < width; j++) {
			board_buf[buf_len - i - 1][j] = _data[sy + start - i][sx + j];
			_data[sy + start - i][sx + j] = 0x00;
		}
		clear_buf[buf_len - i - 1] = clearingBuf[start - i];
		clearingBuf[start - i] = 0;
	}
	for (int i = 0; i < buf_len; i++) {
		if (dest - i < 0 || dest - i >= height)
			continue;
		for (int j = 0; j < width; j++)
			_data[sy + dest - i][sx + j] = board_buf[buf_len - i - 1][j];
		clearingBuf[dest - i] = clear_buf[buf_len - i - 1];
	}
}

Tetris::Next::Next() :
	blocks(ref_blocks), generator(time(NULL)), distribution(0, Block::Type::LENGTH - 1) {}
Tetris::Next::Next(const Next& next) : blocks(ref_blocks), generator(next.generator), distribution(0, Block::Type::LENGTH - 1) {
	block_queue = next.block_queue;
	ref_blocks[0] = next.ref_blocks[0];
	ref_blocks[1] = next.ref_blocks[1];
	ref_blocks[2] = next.ref_blocks[2];
	ref_blocks[3] = next.ref_blocks[3];
	ref_blocks[4] = next.ref_blocks[4];
}
void Tetris::Next::reset() {
	block_queue.clear();
	addBlocks();
}
void Tetris::Next::addBlocks() {
	const int QUEUE_INC_SIZE = 2;
	using B = Block::Type;
	Block::Type new_blocks[] = {
		B::Z,B::L,B::O,B::S,B::I,B::J,B::T,
		B::Z,B::L,B::O,B::S,B::I,B::J,B::T
	};
	// make new_blocks randomly
	auto randomArray = [&]() {
		for (int k = 0; k < QUEUE_INC_SIZE; k++) {
			const int start = k * Block::Type::LENGTH;
			const int end = start + Block::Type::LENGTH;
			for (int i = start; i < end; i++) {
				volatile const int ri = start + distribution(generator);
				// swap
				Block::Type temp = new_blocks[i];
				new_blocks[i] = new_blocks[ri];
				new_blocks[ri] = temp;
			}
		}
	};
	randomArray();
	randomArray();
	// push to block queue
	for (int i = 0; i < QUEUE_INC_SIZE * Block::Type::LENGTH; i++)
		block_queue.push_back(new_blocks[i]);
}
void Tetris::Next::pasteBlocks() {
	ref_blocks[0] = block_queue[0];
	ref_blocks[1] = block_queue[1];
	ref_blocks[2] = block_queue[2];
	ref_blocks[3] = block_queue[3];
	ref_blocks[4] = block_queue[4];
}

Tetris::Hold::Hold() : block(type) {}
Tetris::Hold::Hold(const Hold& hold) : block(type) {
	type = hold.type;
	has_holded = hold.has_holded;
}

Tetris::Controller::Controller(Tetris& tetris) :
	tetris(tetris), _current{}, previous{}, shadow{}, current(_current), last(_last) {}
Tetris::Controller::Controller(const Controller& controller, Tetris& tetris) :
	tetris(tetris), _current{controller._current}, previous{controller.previous},
	shadow{controller.shadow}, _last{controller._last}, current(_current), last(_last) {}
bool Tetris::Controller::holdBlock() {
	if (tetris.hold.has_holded)
		return false;
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	if (tetris.hold.type != Block::NONE)
		tetris.next.block_queue.push_front(tetris.hold.type);
	tetris.hold.type = _current.type;
	generateBlock();
	tetris.hold.has_holded = true;
	return true;
}
int Tetris::Controller::cleanLines() {
	int line_count = 0;
	for (int i = tetris.board.height - 1; i >= 0;) {
		if (tetris.board.clearingBuf[i] == tetris.board.width) {
			tetris.board.eraseLine(i);
			tetris.board.shift(i - 1, i, tetris.board.height);
			line_count++;
		}
		else
			i--;
	}
	return line_count;
}

bool Tetris::Controller::generateBlock(bool update_last) {
	int line_count = cleanLines();
	// update info
	int max_height = 0;
	for (int i = 0; i < tetris.board.width; i++)
		for (int j = 0; j < tetris.board.height; j++)
			if (tetris.board.data[j][i]) {
				const int height = tetris.board.height - j;
				tetris.info._line_height[i] = height;
				if (height > max_height)
					max_height = height;
				break;
			}
	tetris.info._clear_count += line_count;
	tetris.info._max_height = max_height;
	// record last block
	if (update_last)
		_last = _current;
	// set new block property
	const Block::Type type = tetris.next.block_queue.front();
	_current.x = (tetris.board.width - block[type].width) / 2;
	_current.y = -1;
	_current.type = type;
	_current.rot = 0;
	// test if generate block failed
	if (tetris.board.testCollision(_current.x, _current.y, _current.type, _current.rot)) {
		tetris._game_state = false;
		return false;
	}
	tetris.next.block_queue.pop_front();
	// reset previous state
	previous.x = previous.y = previous.rot = -1;
	previous.type = Block::Type::NONE;
	// add new blocks to queue
	if (tetris.next.block_queue.size() < 2 * Block::Type::LENGTH)
		tetris.next.addBlocks();
	// generate block
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	// paste shadow
	pasteShadow();
	// set hold block
	tetris.hold.has_holded = false;
	// paste blocks
	tetris.next.pasteBlocks();
	tetris._game_state = true;
	// increase total blocks
	tetris.info._total_blocks++;
	return true;
}
Tetris::Controller::State Tetris::Controller::getDropPlace() {
	State state = _current;
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	while (true) {
		if (tetris.board.testCollision(state.x, state.y + 1, state.type, state.rot))
			break;
		state.y += 1;
	}
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	return state;
}
void Tetris::Controller::pasteShadow() {
	tetris.board.eraseShadow(shadow.x, shadow.y, shadow.type, shadow.rot);
	shadow = getDropPlace();
	tetris.board.pasteShadow(shadow.x, shadow.y, shadow.type, shadow.rot);
}
bool Tetris::Controller::rotateBlock(int dir) {
	// ref to x, y, type, rot
	auto& x = _current.x;
	auto& y = _current.y;
	auto& type = _current.type;
	auto& rot = _current.rot;
	// test success
	bool moved = false;
	int new_rot = (rot + 5 - 2 * dir) % 4;
	// get wall kick tests
	auto& b = block[type];
	auto& rot_tests = b.srs.test[rot][dir];
	tetris.board.eraseBlock(x, y, type, rot);
	// start testing
	for (int i = 0; i < b.srs.length; i++) {
		const int nx = x + rot_tests[i].x;
		const int ny = y - rot_tests[i].y;
		if (!tetris.board.testCollision(nx, ny, type, new_rot)) {
			x = nx;
			y = ny;
			rot = new_rot;
			moved = true;
			break;
		}
	}
	tetris.board.pasteBlock(x, y, type, rot);
	pasteShadow();
	return moved;
}

bool Tetris::Controller::moveLeft() {
	bool moved = false;
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	if (!tetris.board.testCollision(_current.x - 1, _current.y, _current.type, _current.rot)) {
		_current.x -= 1;
		// reset previous state
		previous.x = previous.y = previous.rot = -1;
		previous.type = Block::Type::NONE;
		moved = true;
	}
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	pasteShadow();
	return moved;
}
bool Tetris::Controller::moveRigh() {
	bool moved = false;
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	if (!tetris.board.testCollision(_current.x + 1, _current.y, _current.type, _current.rot)) {
		_current.x += 1;
		// reset previous state
		previous.x = previous.y = previous.rot = -1;
		previous.type = Block::Type::NONE;
		moved = true;
	}
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	pasteShadow();
	return moved;
}
bool Tetris::Controller::softDrop() {
	bool moved = false;
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	if (!tetris.board.testCollision(_current.x, _current.y + 1, _current.type, _current.rot)) {
		_current.y += 1;
		// reset previous state
		previous.x = previous.y = previous.rot = -1;
		previous.type = Block::Type::NONE;
		moved = true;
	}
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	return moved;
}
bool Tetris::Controller::hardDrop() {
	State state = getDropPlace();
	tetris.board.eraseBlock(_current.x, _current.y, _current.type, _current.rot);
	_current = state;
	tetris.board.pasteBlock(_current.x, _current.y, _current.type, _current.rot);
	return generateBlock(true);
}
bool Tetris::Controller::rotateLeft() {
	return rotateBlock(1);
}
bool Tetris::Controller::rotateRight() {
	return rotateBlock(0);
}
// bool Tetris::Controller::rotate180() {}
bool Tetris::Controller::hold() {
	return holdBlock();
}

int Tetris::Controller::seqMove(const char* move_seq) {
	if (!move_seq) return -1;
	int success_count = 0;
	do {
		switch (*move_seq) {
		case '0':
			success_count += (int)moveLeft();
			break;
		case '1':
			success_count += (int)moveRigh();
			break;
		case '2':
			success_count += (int)softDrop();
			break;
		case '3':
			success_count += (int)hardDrop();
			break;
		case '4':
			success_count += (int)rotateLeft();
			break;
		case '5':
			success_count += (int)rotateRight();
			break;
		case '6':
			success_count += (int)hold();
			break;
		default:
			break;
		}
	} while (*++move_seq);
	return success_count;
}

void Tetris::Controller::start() {
	tetris.next.reset();
	tetris.hold.type = Block::Type::NONE;
	_last.type = Block::Type::NONE;
	generateBlock();
}
bool Tetris::Controller::update() {
	softDrop();
	// if block unchanged, then next block
	if (_current.x == previous.x &&
		_current.y == previous.y &&
		_current.type == previous.type &&
		_current.rot == previous.rot) {
		return generateBlock(true);
	}
	previous = _current;
	return true;
}

Tetris::Features::Features(Tetris& tetris) : tetris(tetris) {}
/*
[board.width]: each line height
[board.width - 1]: diff between each line height
[7]: current block
[4]: current block rot
[1]: distance between current block and left wall
[1]: distance between current block and right wall
[1]: distance between current block and board bottom
[1]: left_remain_dist
[1]: right_remain_dist
[1]: bottom_remain_dist
[7]: first next block
*/
std::vector<int> Tetris::Features::get_learning_features() {
	std::vector<int> feats;
	auto& board = tetris.board;
	// board data with sx sy
	auto& _data = board._data;
	for (int i = 0; i < board.width; i++)
		feats.push_back(tetris.info.line_height[i]);
	for (int i = 0; i < board.width - 1; i++)
		feats.push_back(tetris.info.line_height[i] - tetris.info.line_height[i + 1]);
	for (int i = 0; i < Block::Type::LENGTH; i++)
		feats.push_back(i == static_cast<int>(tetris.controller.current.type));
	for (int i = 0; i < 4; i++)
		feats.push_back(i == static_cast<int>(tetris.controller.current.rot));
	feats.push_back(static_cast<int>(tetris.controller.current.rot));
	// distance between current block and ...
	auto& current = tetris.controller.current;
	if (current.type != Block::Type::NONE) {
		auto& block = Tetris::block[current.type];
		auto& data = block.data[current.rot];
		int left = block.width, right = -1, bottom = -1;
		int left_remain_dist = board.width;
		int right_remain_dist = board.width;
		int bottom_remain_dist = board.height;
		for (int i = 0; i < block.height; i++) {
			// calc left
			for (int j = 0; j < block.width; j++)
				if (data[i][j]) {
					if (j < left)
						left = j;
					// for left_remain_dist
					int void_count = 0;
					for (int k = current.x + j - 1; k >= 0; k--) {
						if (_data[board.sy + current.y + i][board.sx + k])
							break;
						void_count++;
					}
					if (void_count < left_remain_dist)
						left_remain_dist = void_count;
					break;
				}
			// calc right
			for (int j = block.width - 1; j >= 0; j--)
				if (data[i][j]) {
					if (j > right)
						right = j;
					// for right_remain_dist
					int void_count = 0;
					for (int k = current.x + j + 1; k < board.width; k++) {
						if (_data[board.sy + current.y + i][board.sx + k])
							break;
						void_count++;
					}
					if (void_count < right_remain_dist)
						right_remain_dist = void_count;
					break;
				}
		}
		// calc bottom
		for (int j = 0; j < block.width; j++)
			for (int i = block.height - 1; i >= 0; i--)
				if (data[i][j]) {
					if (i > bottom)
						bottom = i;
					// for bottom_remain_dist
					int void_count = 0;
					for (int k = current.y + i + 1; k < board.height; k++) {
						if (_data[board.sy + k][board.sx + current.x + j])
							break;
						void_count++;
					}
					if (void_count < bottom_remain_dist)
						bottom_remain_dist = void_count;
					break;
				}
		feats.push_back(current.x + left);
		feats.push_back(tetris.board.width - (current.x + right) - 1);
		feats.push_back(tetris.board.height - (current.y + bottom) - 1);
		// push remains ...
		feats.push_back(left_remain_dist);
		feats.push_back(right_remain_dist);
		feats.push_back(bottom_remain_dist);
	}
	else {
		feats.push_back(-1);
		feats.push_back(-1);
		feats.push_back(-1);
		// push remains ...
		feats.push_back(-1);
		feats.push_back(-1);
		feats.push_back(-1);
	}
	// first next block
	for (int i = 0; i < Block::Type::LENGTH; i++)
		feats.push_back(i == static_cast<int>(tetris.next.blocks[0]));
	return feats;
}
/*
[board.width]: each line height
[board.width - 1]: diff between each line height
[1]: max line height
[1]: total clear lines count
[1]: last block holes count
[1]: last block place height
[1]: total number of holes below each line top
[1]: game state
[1]: last block compactness
*/
std::vector<int> Tetris::Features::get_reward_features() {
	std::vector<int> feats;
	for (int i = 0; i < tetris.board.width; i++)
		feats.push_back(tetris.info.line_height[i]);
	for (int i = 0; i < tetris.board.width - 1; i++)
		feats.push_back(tetris.info.line_height[i] - tetris.info.line_height[i + 1]);
	feats.push_back(tetris.info.max_height);
	feats.push_back(tetris.info.clear_count);
	auto& last = tetris.controller.last;
	if (last.type != Block::Type::NONE) {
		auto& block = Tetris::block[last.type];
		// calc last block holes count
		int holes_count = 0;
		for (int j = 0; j < block.width; j++)
			for (int bottom = block.height - 1; bottom >= 0; bottom--)
				if (block.data[last.rot][bottom][j]) {
					for (int i = last.y + bottom + 1; i >= 0 && i < tetris.board.height; i++) {
						const int cj = last.x + j;
						if (cj < 0 || cj >= tetris.board.width)
							break;
						if (tetris.board.data[i][cj])
							break;
						holes_count++;
					}
					break;
				}
		feats.push_back(holes_count);
		// calc last block place height
		int top = block.height;
		for (int j = 0; j < block.width; j++)
			for (int _top = 0; _top < block.height; _top++)
				if (block.data[last.rot][_top][j] && _top < top) {
					top = _top;
					break;
				}
		feats.push_back(tetris.board.height - (last.y + top));
	}
	else {
		feats.push_back(0);
		feats.push_back(0);
	}
	// calc the total number of holes below each line top
	int total_holes = 0;
	for (int i = 0; i < tetris.board.width; i++) {
		const int board_height = tetris.board.height;
		for (int j = board_height - tetris.info.line_height[i]; j < board_height; j++)
			if (!tetris.board.data[j][i])
				total_holes++;
	}
	feats.push_back(total_holes);
	feats.push_back(tetris.game_state);
	feats.push_back(last_block_compactness());
	return feats;
}
std::vector<int> Tetris::Features::get_hard_drop_reward_features() {
	Tetris simulator(tetris);
	simulator.controller.hardDrop();
	return simulator.features.get_reward_features();
}

int Tetris::Features::last_block_compactness() {
	static const int di[] = { -1,0,0,1 };
	static const int dj[] = { 0,-1,1,0 };
	auto& last = tetris.controller.last;
	if (last.type == Block::Type::NONE)
		return -1;
	auto& block = Tetris::block[last.type];
	// block data
	auto& data = block.data[last.rot];
	auto& board = tetris.board;
	// board data with sx sy
	auto& _data = board._data;
	UBYTE buf[6][6] = {};
	for (int i = 0; i < block.height; i++)
		for (int j = 0; j < block.width; j++)
			if (data[i][j])
				for (int k = 0; k < 4; k++) {
					const int bi = i + di[k];
					const int bj = j + dj[k];
					const int ci = board.sy + last.y + bi;
					const int cj = board.sx + last.x + bj;
					if (_data[ci][cj])
						buf[bi + 1][bj + 1] = 0x01;
				}
	for (int i = 0; i < block.height; i++)
		for (int j = 0; j < block.width; j++)
			if (data[i][j])
				buf[i + 1][j + 1] = 0x00;
	int compactness = 0;
	for (int i = 0; i < block.height + 2; i++)
		for (int j = 0; j < block.width + 2; j++)
			compactness += buf[i][j];
	return compactness;
}

Tetris::Tetris(int width, int height) :
	game_state(_game_state), info(width), board(width, height), controller(*this), features(*this) {
	start();
}
Tetris::Tetris(const Tetris& tetris) :
	game_state(_game_state), info(tetris.info), board(tetris.board), next(tetris.next), hold(tetris.hold),
	controller(tetris.controller, *this), features(*this) {
	_game_state = tetris._game_state;
}
void Tetris::start() {
	info.reset();
	board.clear();
	controller.start();
	hold.type = Block::NONE;
	hold.has_holded = false;
}
bool Tetris::update() {
	return controller.update();
}

std::string Tetris::dump() {
	static const char* block_style[] = { " "," "," ","Z","L","O","S","I","J","T" };
	std::stringstream dump_buf;
	// board info
	dump_buf << "+";
	for (int i = 0; i < board.width; i++)
		dump_buf << "--";
	dump_buf << "+\n";
	for (int i = 0; i < board.height; i++) {
		dump_buf << "|";
		for (int j = 0; j < board.width; j++) {
			const int temp = board.data[i][j] >> 4;
			int bs = temp >= 0 && temp < 10 ? temp : 0;
			dump_buf << block_style[bs] << block_style[bs];
		}
		dump_buf << "|\n";
	}
	dump_buf << "+";
	for (int i = 0; i < board.width; i++)
		dump_buf << "--";
	dump_buf << "+\n";
	// next block
	dump_buf << "Next blocks: [ ";
	for (int i = 0; i < 5; i++)
		dump_buf << block_style[static_cast<int>(next.blocks[i]) + 3] << " ";
	dump_buf << "]\n";
	dump_buf << "Hold block:  [ " << block_style[static_cast<int>(hold.block) + 3] << " ]\n";
	// game state
	dump_buf << "Game state:  " << (game_state ? "true" : "false") << "\n";
	// info
	dump_buf << "Clear count: " << info.clear_count << "\n";
	dump_buf << "Line height: [ ";
	for (int i = 0; i < info.width; i++)
		dump_buf << info.line_height[i] << ' ';
	dump_buf << "]\n";
	dump_buf << "Max height:  " << info.max_height << "\n";
	return dump_buf.str();
}

// }
