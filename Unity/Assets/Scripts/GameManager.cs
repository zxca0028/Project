using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using TMPro;

public class GameManager : MonoBehaviour
{
    //게임오버 및 스코어 점수와 UI를 관리하는 게임 매니저

    public static GameManager instance;     //게임매니저 싱글턴

    public bool isGameover = false;         //게임오버 상태
    public TextMeshProUGUI scoreText;              //점수를 출력할 UI텍스트
    public GameObject gameOverUI;           //게임오버 시 활성화할 UI게임 오브젝트

    private int score = 0;                  //게임 점수
    private void Awake()
    {
        if (instance == null)
            instance = this; //없다면 자기자신을 할당
        else
        {
            //인스턴스가 이미 있다면
            Debug.LogWarning("씬에 두 개 이상의 게임 매니저가 존재합니다.");
            Destroy(gameObject); //이미 생성된 인스턴스가 있으므로 자기자신을 파괴
        }
    }

    private void Start()
    {
    }

    void Update()
    {
   

        //점수증가
    }

    public void AddScore(int newScore)
    {
        if (!isGameover) //게임오버 상태가 아니라면
        {
            //점수를 증가
            score += newScore;
            scoreText.text = score.ToString();
        }
    }

    public void OnplayerDead()
    {
        isGameover = true;
        gameOverUI.SetActive(true); //게임오버 UI 만들어야 함. 아직 안만들었음
    }

}
