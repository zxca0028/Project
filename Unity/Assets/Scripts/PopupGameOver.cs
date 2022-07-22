using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.SceneManagement;

public class PopupGameOver : MonoBehaviour
{

    private void Start()
    {
        
    }

    public void OnClickedRetryBtn()
    {
        SceneManager.LoadScene(1);
    }
    public void OnClickedExitBtn()
    {
        SceneManager.LoadScene(0);
    }
}
