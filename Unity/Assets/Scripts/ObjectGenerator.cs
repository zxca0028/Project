using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectGenerator : MonoBehaviour
{
    public BlockController      Block_1x;
    public BlockController      Block_2x;
    public BlockController      Block_3x;
    public BlockController      Block_4x;
    public BackGroundController BGPrefab;

    private BlockController      Block;
    private BackGroundController BG;

    private enum BLOCKSIZE
    {
        Various,
        _1x,
        _2x,
        _3x,
        _4x
    }
    private enum DIR
    {
        RIGHT,
        LEFT
    }

    private Queue<BlockController> Blocks;
    private Queue<BackGroundController> BGs;

    private BLOCKSIZE GeneratedBlockSize;
    private DIR       PrevDir;
    private bool      isRandom        = true;
    private int       iFloor          = 6;
    private float     fPrevX          = 0f;
    private float     fHeightInterval = 5f;
    private float     fGroundPos      = -1f;

    void Start()
    {
        Blocks = new Queue<BlockController>();
        BGs    = new Queue<BackGroundController>();

        StaticBlockGenerator();
        StaticBGGenerator();
    }
    void Update()
    {
        DynamicBlockGenerator();
        DynamicBGGenerator();
    }

    private void StaticBGGenerator()
    {
        for(int i = 0; i < 3; ++i)
        {
            BG = Instantiate(BGPrefab) as BackGroundController;

            BG.transform.position = new Vector3(0f, 9f + 19.4f * i, 0f);

            BGs.Enqueue(BG);
        }
    }

    private void DynamicBGGenerator()
    {
        if(false == BGs.Peek().gameObject.activeInHierarchy)
        {
            BackGroundController FirstBG = BGs.Peek();

            FirstBG.gameObject.SetActive(true);

            FirstBG.transform.Translate(0f, 19.4f * 3, 0f);

            BGs.Enqueue(FirstBG);

            BGs.Dequeue();
        }
    }

    private void BlockGenerator()
    {
        float fRandomSize = Random.Range(0f, 1f);
        float fRandomMove = Random.Range(0f, 1f);

        if (fRandomSize <= 0.3f)
        {
            Block = Instantiate(Block_1x) as BlockController;
            GeneratedBlockSize = BLOCKSIZE._1x;

            if(fRandomMove < 0.93f)
            {
                Block.SetMoveState();
            }
        }
        else if (fRandomSize <= 0.7f)
        {
            Block = Instantiate(Block_2x) as BlockController;
            GeneratedBlockSize = BLOCKSIZE._2x;

            if (fRandomMove < 0.03f)
            {
                Block.SetMoveState();
            }
        }
        else
        {
            Block = Instantiate(Block_3x) as BlockController;
            GeneratedBlockSize = BLOCKSIZE._3x;
        }
        //else
        //{
        //    Block = Instantiate(Block_4x) as BlockController;
        //    GeneratedBlockSize = BLOCKSIZE._4x;
        //}
    }

    #region DYNAMIC_BLOCK_GENERATOR
    private void DynamicBlockGenerator()
    {
        if(null == Blocks.Peek())
        {
            Blocks.Dequeue();

            BlockGenerator();

            if(false == isRandom)
            {
                float fRandomX = 0f;

                if (PrevDir == DIR.RIGHT)
                {
                    if (GeneratedBlockSize == BLOCKSIZE._1x)
                    {
                        fRandomX = Random.Range(-5f, -0.7f);
                    }
                    else if (GeneratedBlockSize == BLOCKSIZE._2x)
                    {
                        fRandomX = Random.Range(-4.25f, -1.5f);
                    }
                    else if (GeneratedBlockSize == BLOCKSIZE._3x)
                    {
                        fRandomX = Random.Range(-3.55f, -2.10f);
                    }
                    else
                    {
                        fRandomX = -2.85f;
                    }
                }
                else
                {
                    if (GeneratedBlockSize == BLOCKSIZE._1x)
                    {
                        fRandomX = Random.Range(0.7f, 5f);
                    }
                    else if (GeneratedBlockSize == BLOCKSIZE._2x)
                    {
                        fRandomX = Random.Range(1.5f, 4.25f);
                    }
                    else if (GeneratedBlockSize == BLOCKSIZE._3x)
                    {
                        fRandomX = Random.Range(2.1f, 3.55f);
                    }
                    else
                    {
                        fRandomX = 2.85f;
                    }
                }

                Block.transform.position = new Vector3(fRandomX, fHeightInterval * iFloor + fHeightInterval + fGroundPos);

                if (Block.transform.position.x > 0)
                {
                    PrevDir = DIR.RIGHT;
                }
                else
                {
                    PrevDir = DIR.LEFT;
                }

            }
            else
            {
                do
                {
                    Block.transform.position = new Vector3(Random.Range(-5f, 5f), fHeightInterval * iFloor + fHeightInterval + fGroundPos);
                }
                while (Mathf.Abs(fPrevX - Block.transform.position.x) < 2f);

                fPrevX = Block.transform.position.x;
            }

            ++iFloor;

            Blocks.Enqueue(Block);

        }
    }
    #endregion

    #region STATIC_BLOCK_GENERATOR
    void StaticBlockGenerator()
    {
        for (int i = 0; i < iFloor; ++i)
        {
            BlockGenerator();

            if(false == isRandom)
            {
                if (i == 0)
                {
                    Block.transform.position = new Vector3(Random.Range(-5f, 5f), fHeightInterval * i + fHeightInterval + fGroundPos);
                }
                else
                {
                    float fRandomX = 0f;

                    if (PrevDir == DIR.RIGHT)
                    {
                        if (GeneratedBlockSize == BLOCKSIZE._1x)
                        {
                            fRandomX = Random.Range(-5f, -0.7f);
                        }
                        else if (GeneratedBlockSize == BLOCKSIZE._2x)
                        {
                            fRandomX = Random.Range(-4.25f, -1.5f);
                        }
                        else if (GeneratedBlockSize == BLOCKSIZE._3x)
                        {
                            fRandomX = Random.Range(-3.55f, -2.10f);
                        }
                        else
                        {
                            fRandomX = -2.85f;
                        }
                    }
                    else
                    {
                        if (GeneratedBlockSize == BLOCKSIZE._1x)
                        {
                            fRandomX = Random.Range(0.7f, 5f);
                        }
                        else if (GeneratedBlockSize == BLOCKSIZE._2x)
                        {
                            fRandomX = Random.Range(1.5f, 4.25f);
                        }
                        else if (GeneratedBlockSize == BLOCKSIZE._3x)
                        {
                            fRandomX = Random.Range(2.1f, 3.55f);
                        }
                        else
                        {
                            fRandomX = 2.85f;
                        }
                    }

                    Block.transform.position = new Vector3(fRandomX, fHeightInterval * i + fHeightInterval + fGroundPos);
                }

                if (Block.transform.position.x > 0)
                {
                    PrevDir = DIR.RIGHT;
                }
                else
                {
                    PrevDir = DIR.LEFT;
                }
            }
            else
            {
                if (i == 0)
                {
                    Block.transform.position = new Vector3(Random.Range(-5f, 5f), fHeightInterval * i + fHeightInterval + fGroundPos);
                }
                else
                {
                    do
                    {
                        Block.transform.position = new Vector3(Random.Range(-5f, 5f), fHeightInterval * i + fHeightInterval + fGroundPos);
                    }
                    while (Mathf.Abs(fPrevX - Block.transform.position.x) < 2f);
                }

                fPrevX = Block.transform.position.x;
            }

            Blocks.Enqueue(Block);
        }
    }
    #endregion
}
