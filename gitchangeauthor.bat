
git filter-branch --env-filter '

an="$GIT_AUTHOR_NAME"
am="$GIT_AUTHOR_EMAIL"
cn="$GIT_COMMITTER_NAME"
cm="$GIT_COMMITTER_EMAIL"

if [ "$GIT_COMMITTER_EMAIL" = "80308920@adc.com" ]
then
    cn="kevin3134"
    cm="kevinliminhao@gmail.com"
fi
if [ "$GIT_AUTHOR_EMAIL" = "80308920@adc.com" ]
then
    an="kevin3134"
    am="kevinliminhao@gmail.com"
fi

export GIT_AUTHOR_NAME="$an"
export GIT_AUTHOR_EMAIL="$am"
export GIT_COMMITTER_NAME="$cn"
export GIT_COMMITTER_EMAIL="$cm"
'