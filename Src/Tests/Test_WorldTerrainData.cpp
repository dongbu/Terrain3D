#include <gtest/gtest.h>
#include <World/Terrain/Data.h>

using namespace t3d::World::Terrain;

class Test_WorldTerrainData : public ::testing::Test
{
protected:
	void testComputeTextureIndiciesBasic(int size, float baseHeight, int expectedIndex, Data::HeightIndex &heightIndex)
	{
		Data data(1);
		data.heightMap().reserve(size);

		for (int y=0; y<size; y++)
		{
			for (int x=0; x<size; x++)
				data.heightMap().set(x, y, baseHeight);
		}

		data.computeTextureIndicies(heightIndex);

		std::vector<GLubyte> ti = data.textureIndicies();
		for (unsigned i=0; i<ti.size(); i++)
			ASSERT_EQ(expectedIndex, ti[i]);
	}
};


TEST_F(Test_WorldTerrainData, computeTextureIndicies1)
{
	Data::HeightIndex hi;
	hi[0.5f] = 0;
	hi[0.7f] = 1;

	testComputeTextureIndiciesBasic(2, 0.0f, 0, hi);
}


TEST_F(Test_WorldTerrainData, computeTextureIndicies2)
{
	Data::HeightIndex hi;
	hi[0.1f] = 0;
	hi[0.7f] = 4;
	hi[1.0f] = 7;

	testComputeTextureIndiciesBasic(5, 140.0f, 4, hi);
}


TEST_F(Test_WorldTerrainData, computeTextureIndicies3)
{
	const int size = 2;
	const int res = 2;

	float heights[size][size] =
	{
		{ 127.0f, 191.0f },
		{ 63.0f, 255.0f }
	};

	Data data(res);
	data.heightMap().reserve(size);

	for (int y=0; y<size; y++)
	{
		for (int x=0; x<size; x++)
		{
			data.heightMap().set(x, y, heights[y][x]);
		}
	}

	const int tciSize = (size-1) * res + 1;
	GLubyte expected[tciSize*tciSize] =
	{
		1, 2, 2,
		1, 2, 3,
		0, 2, 3
	};

	Data::HeightIndex hi;
	hi[0.25f] = 0;
	hi[0.50f] = 1;
	hi[0.75f] = 2;
	hi[1.00f] = 3;

	data.computeTextureIndicies(hi);

	//verify
	for (int i=0; i<data.textureIndicies().size(); i++)
	{
		ASSERT_EQ(expected[i], data.textureIndicies()[i]);
	}
}