using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using TMPro;

public class GameManager : MonoBehaviour
{
    //���ӿ��� �� ���ھ� ������ UI�� �����ϴ� ���� �Ŵ���

    public static GameManager instance;     //���ӸŴ��� �̱���

    public bool isGameover = false;         //���ӿ��� ����
    public TextMeshProUGUI scoreText;              //������ ����� UI�ؽ�Ʈ
    public GameObject gameOverUI;           //���ӿ��� �� Ȱ��ȭ�� UI���� ������Ʈ

    private int score = 0;                  //���� ����
    private void Awake()
    {
        if (instance == null)
            instance = this; //���ٸ� �ڱ��ڽ��� �Ҵ�
        else
        {
            //�ν��Ͻ��� �̹� �ִٸ�
            Debug.LogWarning("���� �� �� �̻��� ���� �Ŵ����� �����մϴ�.");
            Destroy(gameObject); //�̹� ������ �ν��Ͻ��� �����Ƿ� �ڱ��ڽ��� �ı�
        }
    }

    private void Start()
    {
    }

    void Update()
    {
   

        //��������
    }

    public void AddScore(int newScore)
    {
        if (!isGameover) //���ӿ��� ���°� �ƴ϶��
        {
            //������ ����
            score += newScore;
            scoreText.text = score.ToString();
        }
    }

    public void OnplayerDead()
    {
        isGameover = true;
        gameOverUI.SetActive(true); //���ӿ��� UI ������ ��. ���� �ȸ������
    }

}
