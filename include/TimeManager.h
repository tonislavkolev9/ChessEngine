#pragma once

void startSearchTimer(long long milliseconds);

void clearSearchTimer();

bool isSearchTimeUp();

void requestSearchStop();

void clearSearchStop();

struct SearchTimeout {};
