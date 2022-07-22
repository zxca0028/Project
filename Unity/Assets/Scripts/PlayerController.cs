using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    #region VARIABLE

    private enum STATE 
    { 
        STATE_IDLE,
        STATE_ARROW_GENERATE,
        STATE_ARROW,
        STATE_GAUGE_GENERATE,
        STATE_GAUGE,
        STATE_JUMPING,
        STATE_FLYING,
        STATE_LANDING,
        STATE_DIE,
        STATE_END
    };

    private Vector2 vDir;
    private STATE   eState           = STATE.STATE_FLYING;
    private int     iReflectCnt      = 0;
    private float   fPower           = 0f;
    private float   fTimeAcc         = 0f;
    private float   fDist            = 0f;
    private bool    bStart           = false;
    private bool    isGrounded       = false;
    private bool    isDead           = false;
    private bool    isOnMovableBlock = false;


    [SerializeField]
    private ArrowController ArrowPrefab;
    private ArrowController Arrow;

    private Transform BlockTransform;

    private Animator    animator;
    public  AudioClip   deathClip;
    private Rigidbody2D playerRigidbody;
    private AudioSource playerAudio;

    #endregion

    void Start()
    {
        playerRigidbody = GetComponent<Rigidbody2D>();
        animator        = GetComponent<Animator>();
        playerAudio     = GetComponent<AudioSource>();

        bStart = true;

        Time.timeScale = 1;
    }

    void Update()
    {
        if(true == isGrounded)
        {
            transform.position = new Vector3(BlockTransform.position.x + fDist, transform.position.y, transform.position.z);
        }
        if(transform.position.y < 3f)
        {
            isOnMovableBlock = false;
        }

        PlayerBehavior();
    }

    public void SetState(bool state)
    {
        isOnMovableBlock = state;
    }

    #region COLLISION

    private void OnCollisionEnter2D(Collision2D collision)
    {
        //첫번째로 충돌한 노말벡터의 y축 방향이 0.7이상일 경우(위쪽 방향일 경우)
        if(collision.contacts[0].normal.y > 0.7f)
        {
            isGrounded  = true;      //땅위에 있다
            iReflectCnt = 0;
            eState      = STATE.STATE_LANDING;

            BlockTransform = collision.contacts[0].collider.transform;

            fDist = transform.position.x - BlockTransform.position.x;
        }

        if(collision.contacts[0].normal.x == 1)
        {
            transform.position = new Vector3(transform.position.x + 0.02f, transform.position.y, transform.position.z);
        }
        else if(collision.contacts[0].normal.x == -1)
        {
            transform.position = new Vector3(transform.position.x - 0.02f, transform.position.y, transform.position.z);
        }

        
    }

    private void OnCollisionExit2D(Collision2D collision)
    {
        isGrounded = false;
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == "Dead" && !isDead)
        {
            eState = STATE.STATE_DIE;
        }     
        if(collision.tag == "ReflectR" || collision.tag == "ReflectL")
        {
            playerAudio.Play();

            Vector3 vNormal;

            if(collision.tag == "ReflectR")
            {
                vNormal = new Vector3(-1f, 0f, 0f);
            }
            else
            {
                vNormal = new Vector3(1f, 0f, 0f);
            }

            if(iReflectCnt == 1)
            {
                vDir.y *= -1;
            }

            Vector3 vReflect = Vector3.Reflect(vDir, vNormal);
            
            Vector3.Normalize(vDir);

            transform.right = new Vector3(-vDir.x, 0f, 0f);

            playerRigidbody.velocity = Vector2.zero;

            playerRigidbody.AddForce(vReflect * fPower);

            vDir = vReflect;

            ++iReflectCnt;
        }
    }

    #endregion

    #region BEHAVIOR

    private void PlayerBehavior()
    {
        switch (eState)
        {
            case STATE.STATE_IDLE:

                fTimeAcc += Time.deltaTime;

                if (fTimeAcc > .2f)
                {
                    eState = STATE.STATE_ARROW_GENERATE;

                    fTimeAcc = 0f;
                }

                break;

            case STATE.STATE_ARROW_GENERATE:

                Arrow = Instantiate(ArrowPrefab) as ArrowController;

                Arrow.SetPlayerTransform(transform);
                
                if(true == isOnMovableBlock)
                {
                    Arrow.SetState();
                }

                Arrow.transform.position = this.transform.position;

                eState = STATE.STATE_ARROW;

                break;

            case STATE.STATE_ARROW:

                if (Input.GetMouseButtonUp(0))
                {
                    Arrow.AngleCorrection();

                    vDir = Arrow.transform.up;

                    Vector3.Normalize(vDir);

                    transform.right = new Vector3(vDir.x, 0f, 0f);

                    fPower = Arrow.transform.localScale.y * 800f * 1.05f;

                    eState = STATE.STATE_JUMPING;
                }

                break;

            case STATE.STATE_JUMPING:

                fTimeAcc += Time.deltaTime;

                if (fTimeAcc > .1f)
                {
                    playerRigidbody.velocity = Vector2.zero;

                    playerRigidbody.AddForce(vDir * fPower);
                    Destroy(Arrow.gameObject);

                    eState = STATE.STATE_FLYING;

                    playerAudio.Play();

                    isGrounded = false;
                    animator.SetBool("Grounded", isGrounded);

                    fTimeAcc = 0f;
                }

                break;

            case STATE.STATE_FLYING:
                break;

            case STATE.STATE_LANDING:

                animator.SetBool("Grounded", isGrounded);

                playerRigidbody.velocity = Vector2.zero;

                //GameManager.instance.AddScore(100);

                if (true == bStart)
                {
                    Arrow = Instantiate(ArrowPrefab) as ArrowController;

                    Arrow.SetPlayerTransform(transform);

                    Arrow.transform.position = this.transform.position;

                    eState = STATE.STATE_ARROW;

                    fTimeAcc = 0;

                    bStart = false;

                    break;
                }
                else
                {
                    fTimeAcc += Time.deltaTime;
                }
                if (fTimeAcc > .5f && true == isGrounded)
                {
                    eState = STATE.STATE_IDLE;

                    fTimeAcc = 0f;
                }

                break;

            case STATE.STATE_DIE:

                //오디오 소스에 할당된 오디오 클립을 deathClip 으로 변경
                playerAudio.clip = deathClip;

                playerAudio.Play();

                //속도를 제로로 변경
                playerRigidbody.velocity = Vector2.zero;

                //사망 상태를 true 변경
                isDead = true;

                //게임 매니저의 게임오버 처리
                GameManager.instance.OnplayerDead();

                //GameOverPage = Instantiate(GameOverPagePrefab) as GameOverMenu;

                //Vector3 vPos = new Vector3(0f, transform.position.y + 10f, 0f);

                //GameOverPage.transform.position = vPos;

                Time.timeScale = 0f;

                break;
        }
    }

    #endregion
}
