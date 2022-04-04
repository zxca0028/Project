#pragma once

namespace COOKIE
{
	enum TYPE
	{
		CREAM,
		ORANGE,
		BLACKBERRY,
		END
	};
}

namespace OBJECT
{
	enum TYPE
	{
		BACKGROUND,
		BACKGROUND_2,
		GROUND,
		BT_BACKGROUND,
		BT_GROUND,
		ITEM,
		OBSTACLE,
		JELLY,
		UI,
		EFFECT,
		FLAG,
		PLAYER,
		PET,
		SCORE,
		END
	};
}

namespace SCENE
{
	enum TYPE
	{
		LOADING,
		LOBBY,
		COOKIESELECT,
		STAGELOADING,
		STAGE,
		PAUSE,
		RESULT,
		RENDERTEST,
		END
	};
}

namespace PLAYER
{
	enum STATE
	{
		RUN,
		DASH,
		JUMP,
		D_JUMP,
		SLIDE,
		CRASH,
		FLYING,
		UP,
		DOWN,
		EXHAUST,
		INTRO
	};
}

namespace OBSTACLE
{
	enum TYPE
	{
		TOP_1,
		TOP_2,
		TOP_3,
		BOTTOM_1_J1,
		BOTTOM_2_J1,
		BOTTOM_1_J2,
		BOTTOM_2_J2,
		END
	};
}

namespace ITEM
{
	enum TYPE
	{
		BLAST_JELLY,
		MAGNET_JELLY,
		BIGGEST_JELLY,
		ENERGY_JELLY,
		BIG_ENERGY_JELLY,
		B,
		O,
		N,
		U,
		S,
		T,
		I,
		M,
		E,
		BLAST_EFFECT,
		JELLY_EFFECT
	};
}

namespace JELLY
{
	enum TYPE
	{
		BIG_GOLD_BEAR,
		BIG_PINK_BEAR,
		RAINBOW_BEAR,
		BIG_SILVER_COIN,
		BIG_GOLD_COIN,
		SILVER_COIN,
		GOLD_COIN,
		GOLD_BEAR,
		PINK_BEAR,
		NORMAL,
		END
	};
}

namespace UI
{
	enum TYPE
	{
		BONUSTIME,
		GAUGEICON,
		GAUGE,
		PAUSE,
		END
	};
}

namespace STAGE
{
	enum TYPE
	{
		NORMAL,
		BT_IN,
		BT_OUT_1,
		BT_OUT_2,
		BT_OUT_3,
		BONUSTIME,
		END
	};
}

namespace EFFECT
{
	enum TYPE
	{
		BLAST,
		HEAL,
		BIGGEST,
		MAGNET,
		BLAST_FIRE,
		END
	};
}

namespace BACKGROUND
{
	enum TYPE
	{
		BG_1,
		BG_2,
		END
	};
}

namespace GROUND
{
	enum TYPE
	{
		NORMAL,
		_VOID,
		_VOID2,
		BONUSTIME,
		END
	};
}

namespace PHASE
{
	enum TYPE
	{
		_1ST,
		_2ND,
		_3RD,
		_4TH
	};
}