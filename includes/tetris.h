#ifndef TETRIS_H
#define TETRIS_H

#include <vector>
#include <deque>
#include <string>
#include <random>

// namespace TetrisAPI {

using UBYTE = unsigned char;

struct Coordinate {
	int x, y;
};

class Tetris {

	bool _game_state;

public:

	const bool& game_state;

	struct Block {

		enum Type {
			NONE = -1,
			Z = 0, L, O, S, I, J, T,
			LENGTH
		} type;
		int width, height;
		UBYTE** data[4];
		struct SRS {

			const Coordinate* test[4][2];
			int length;

			SRS(Type type);

		} srs;

		Block(Type type, int width, int height, std::vector<std::vector<std::vector<UBYTE>>> data);
		~Block();

	};

	static const Block block[Block::Type::LENGTH];

	class Info {

		friend class Tetris;

		int width;

		int* _line_height;
		int _max_height;
		int _clear_count;
		int _total_blocks;

	public:

		int*& line_height;
		const int& max_height;
		const int& clear_count;
		const int& total_blocks;

	private:

		Info(int width);
		Info(const Info& info);
		~Info();

		void reset();

	} info;

	class Board {

		friend class Tetris;

		UBYTE** _data;
		UBYTE** _shadow;
		UBYTE* clearingBuf;
		const int sx, sy;

	public:

		const int width, height;

	private:

		// for shift
		UBYTE** board_buf;
		UBYTE* clear_buf;

		// for user
		const UBYTE* const* ref_data;
		const UBYTE* const* ref_shadow;

	public:

		const UBYTE* const*& data;
		const UBYTE* const*& shadow;

	private:

		Board(int width, int height, int sx = 6, int sy = 6);
		Board(const Board& board);
		~Board();

		void clear();
		void pasteBlock(int x, int y, Block::Type type, int rot);
		void eraseBlock(int x, int y, Block::Type type, int rot);
		void pasteShadow(int x, int y, Block::Type type, int rot);
		void eraseShadow(int x, int y, Block::Type type, int rot);
		int testCollision(int x, int y, Block::Type type, int rot);
		void eraseLine(int line);
		void shift(int start, int dest, int lines);

	} board;

	class Next {

		friend class Tetris;

		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution;

		std::deque<Block::Type> block_queue;

		// for user
		Block::Type ref_blocks[5];

	public:

		const Block::Type(&blocks)[5];

	private:

		Next();
		Next(const Next& next);

		void reset();
		void addBlocks();

		void pasteBlocks();

	} next;

	class Hold {

		friend class Tetris;

		Block::Type type;
		bool has_holded;

	public:

		const Block::Type& block;

	private:

		Hold();
		Hold(const Hold& hold);

	} hold;

	class Controller {

		friend class Tetris;

		Tetris& tetris;

		struct State {
			int x, y;
			Block::Type type;
			int rot;
		};

		State _current, previous;
		State shadow;

		State _last;

	public:

		const State& current;
		const State& last;
	
	private:

		Controller(Tetris& tetris);
		Controller(const Controller& controller, Tetris& tetris);

		bool holdBlock();
		int cleanLines();
		bool generateBlock(bool update_last = false);
		State getDropPlace();
		void pasteShadow();
		bool rotateBlock(int dir);

	public:

		bool moveLeft();
		bool moveRigh();
		bool softDrop();
		bool hardDrop();
		bool rotateLeft();
		bool rotateRight();
		// bool rotate180();
		bool hold();

		int seqMove(const char* move_seq);

	private:

		void start();
		bool update();

	} controller;

	class Features {

		Tetris& tetris;

	public:

		Features(Tetris& tetris);

		std::vector<int> get_learning_features();

		std::vector<int> get_reward_features();

		std::vector<int> get_hard_drop_reward_features();

		int last_block_compactness();

	} features;

	Tetris(int width, int height);
	Tetris(const Tetris& tetris);

	void start();
	bool update();

	std::string dump();

};

// }

#endif
