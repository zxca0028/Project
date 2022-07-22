using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TitleMgr : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void startStroyMode()
    {
        SceneManager.LoadScene(1);    //버튼 클릭 이벤트. 메인 게임 씬으로 연결
    }
}
