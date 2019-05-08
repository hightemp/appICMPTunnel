#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

struct Positions
{
    vector<int> oPeople;
    vector<int> oBribes;
};

bool fnGenerateBribesCountList2(
    int iStepId,
    vector<int> &oStepsCountList,
    Positions &oPositions
) 
{
    static int iMaxSteps = 999999;
    int iLength = oPositions.oPeople.size();
    bool bIsEqual = true;

    for (int iIndex=0; iIndex<iLength; iIndex++) {
        bIsEqual &= iIndex+1 == oPositions.oPeople[iIndex];
    }

    if (bIsEqual) {
        //printf("if (bIsEqual) %d %d %d\n", iMaxSteps, iStepId, oStepsCountList[iStepId]);
        if (oStepsCountList.size()) {
            iMaxSteps = oStepsCountList[iStepId];
            //printf("if (bIsEqual && oStepsCountList.size()) %d %d %d\n", iMaxSteps, iStepId, oStepsCountList[iStepId]);
        }
        return true;
    }
    
    bool bBribesRunOut = true;
    bool bResult = false;
    printf("%d %d\n", iStepId,  oStepsCountList[iStepId]);
    for (int i=0; i<iLength; i++) {
        printf("%d,", oPositions.oPeople[i]);
    }
    printf("\n");
    for (int i=0; i<iLength; i++) {
        printf("%d,", oPositions.oBribes[i]);
    }
    printf("\n");
    for (int iIndex=0; iIndex<iLength-1; iIndex++) {
        if (oStepsCountList.size() && oStepsCountList[iStepId]+1>=iMaxSteps) {
            bBribesRunOut = true;
            continue;
        }

        bBribesRunOut &= oPositions.oBribes[iIndex]>=2;

        if (oPositions.oBribes[iIndex]>=2 || oPositions.oPeople[iIndex] <= iIndex+1) {
            //printf("if (oPositions.oBribes[%d]>=2) {\n", iIndex);
            continue;
        }
        Positions oNewPositions = oPositions;

        int iTemp;

        iTemp = oNewPositions.oPeople[iIndex+1];
        oNewPositions.oPeople[iIndex+1] = oNewPositions.oPeople[iIndex];
        oNewPositions.oPeople[iIndex] = iTemp;

        iTemp = oNewPositions.oBribes[iIndex+1];
        oNewPositions.oBribes[iIndex+1] = oNewPositions.oBribes[iIndex]+1;
        oNewPositions.oBribes[iIndex] = iTemp;

        int iNewStepId = iStepId;
        int iPreviousStepsCount = oStepsCountList.size() ? oStepsCountList[iStepId] : 0;

        oStepsCountList.push_back(iPreviousStepsCount+1);
        iNewStepId = oStepsCountList.size()-1;

        bResult |= fnGenerateBribesCountList2(
            iNewStepId, 
            oStepsCountList, 
            oNewPositions
        );
    }

    if (oStepsCountList.size() && oStepsCountList[iStepId]+1>=iMaxSteps) {
        bBribesRunOut = true;
    }

    if (bBribesRunOut || !bResult) {
        //printf("stop %d %d\n", iStepId, oStepsCountList[iStepId]);
        oStepsCountList[iStepId] = 999999;
    }

    return false;
}

bool fnGenerateBribesCountList(
    vector<int> &oFinalState,
    int iStepId,
    vector<int> &oStepsCountList,
    Positions &oPositions
) 
{
    //printf("%d %d\n", iStepId, oStepsCountList[iStepId]);
    static int iMaxSteps = 999999;
    int iLength = oFinalState.size();
    bool bIsEqual = true;

    for (int iIndex=0; iIndex<iLength; iIndex++) {
        bIsEqual &= oFinalState[iIndex] == oPositions.oPeople[iIndex];
    }

    if (bIsEqual) {
        //printf("if (bIsEqual) %d %d %d\n", iMaxSteps, iStepId, oStepsCountList[iStepId]);
        if (oStepsCountList.size()) {
            iMaxSteps = oStepsCountList[iStepId];
            //printf("if (bIsEqual && oStepsCountList.size()) %d %d %d\n", iMaxSteps, iStepId, oStepsCountList[iStepId]);
        }
        return true;
    }

    bool bBribesRunOut = true;
    bool bResult = false;
    for (int iIndex=1; iIndex<iLength; iIndex++) {
        if (oStepsCountList.size() && oStepsCountList[iStepId]+1>=iMaxSteps) {
            bBribesRunOut = true;
            continue;
        }

        bBribesRunOut &= oPositions.oBribes[iIndex]>=2;

        if (oPositions.oBribes[iIndex]>=2) {
            //printf("if (oPositions.oBribes[%d]>=2) {\n", iIndex);
            continue;
        }
        Positions oNewPositions = oPositions;

        int iTemp;

        iTemp = oNewPositions.oPeople[iIndex-1];
        oNewPositions.oPeople[iIndex-1] = oNewPositions.oPeople[iIndex];
        oNewPositions.oPeople[iIndex] = iTemp;

        iTemp = oNewPositions.oBribes[iIndex-1];
        oNewPositions.oBribes[iIndex-1] = oNewPositions.oBribes[iIndex]+1;
        oNewPositions.oBribes[iIndex] = iTemp;

        int iNewStepId = iStepId;
        int iPreviousStepsCount = oStepsCountList.size() ? oStepsCountList[iStepId] : 0;

        oStepsCountList.push_back(iPreviousStepsCount+1);
        iNewStepId = oStepsCountList.size()-1;

        bResult |= fnGenerateBribesCountList(
            oFinalState, 
            iNewStepId, 
            oStepsCountList, 
            oNewPositions
        );
    }

    if (oStepsCountList.size() && oStepsCountList[iStepId]+1>=iMaxSteps) {
        bBribesRunOut = true;
    }

    if (bBribesRunOut || !bResult) {
        //printf("stop %d %d\n", iStepId, oStepsCountList[iStepId]);
        oStepsCountList[iStepId] = 999999;
    }

    return false;
}

/*
int fnGetBribesCount(
    vector<int> &oFinalState
) 
{
    int iLength = oFinalState.size();
    int iStepsCount = 0;

    vector<int> oBribesCount;

    for (int iIndex=0; iIndex<iLength; iIndex++) {
        oBribesCount.push_back(0);
    }

    while (true) {
        int iCurrentManIndex = -1;

        for (int iIndex=0; iIndex<iLength; iIndex++) {
            //printf("> %d %d\n", oFinalState[iIndex], iIndex+1);
            if (oFinalState[iIndex]!=iIndex+1) {
                if (oBribesCount[iIndex]>=2) {
                    return 0;
                }
                iCurrentManIndex = iIndex;
                break;
            }
        }

        if (iCurrentManIndex==-1) {
            //printf("%d %d\n", iCurrentManIndex, iStepsCount);
            return iStepsCount;
        }

        while (oFinalState[iCurrentManIndex]!=iCurrentManIndex+1
                && oBribesCount[iCurrentManIndex]<2) {
            //printf("- %d %d\n", iCurrentManIndex, iStepsCount);
            int iNewCurrentManIndex = iCurrentManIndex+1;
            int iTmp;

            iTmp = oFinalState[iCurrentManIndex];
            oFinalState[iCurrentManIndex] = oFinalState[iNewCurrentManIndex];
            oFinalState[iNewCurrentManIndex] = iTmp;

            iTmp = oBribesCount[iCurrentManIndex];
            oBribesCount[iCurrentManIndex] = oBribesCount[iNewCurrentManIndex];
            oBribesCount[iNewCurrentManIndex] = iTmp+1;

            iCurrentManIndex = iNewCurrentManIndex;
            iStepsCount++;
        }
    }
}
*/

// Complete the minimumBribes function below.
void minimumBribes(vector<int> q) 
{
    vector<int> oStepsCountList = {0};
    Positions oPositions;
    oPositions.oPeople = q;
    for (int iIndex=0; iIndex<q.size(); iIndex++) {
        //oPositions.oPeople.push_back(iIndex+1);
        oPositions.oBribes.push_back(0);
    }
    //fnGenerateBribesCountList(q, 0, oStepsCountList, oPositions);
    fnGenerateBribesCountList2(0, oStepsCountList, oPositions);

    int iResult = 999999;
    //printf("oStepsCountList.size() %d\n", oStepsCountList.size());
    for (int iIndex=0; iIndex<oStepsCountList.size(); iIndex++) {
        //if (oStepsCountList[iIndex]==1) {
            //printf("oStepsCountList[iIndex]==1 %d %d\n", oStepsCountList[iIndex], iIndex);
        //}
        iResult = min(iResult, oStepsCountList[iIndex]);
    }
    
    //int iResult = fnGetBribesCount(q);

    if (iResult == 999999) {
        printf("Too chaotic\n");
    } else {
        printf("%d\n", iResult);
    }
}

int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split_string(q_temp_temp);

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        minimumBribes(q);
    }

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
