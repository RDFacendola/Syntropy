# How to Git

## Basic commands

| Command | Description |
| :------ | :---------- |
| `git init` | Create a new repository at current location. |
| `git clone [url]` | Clone a remote repository at `[url]`. |
| `git status` | Query current git status. |
| `git log` | Shows repository's history. |

## Commits

| Command | Description |
| :------ | :---------- |
| `git add [file]` | Add `[file]` to the staging area.|
| `git add .` | Add _all_ files to the staging area. |
| `git commit -m [message]` | Commit all files in the staging area. |
| `git commit --amend ` | Integrate the previous commit with new changes. |
| `git commit -a ` | Stage everything and commit. |
| `git checkout [hash]` | Return to a previous commit by `[hash]`. |
| `git fetch [remote]` | Pull files from `[remote]` w\o merging. |

## Branching

| Command | Description |
| :------ | :---------- |
| `git branch` | List all branches. |
| `git branch [branch]` | Create a new local branch `[branch]` from current. |
| `git checkout [branch]` | Move to `[branch]`. |
| `git merge [branch]` | Merge `[branch]` to the current branch. |
| `git push [remote] [branch]` | Push local changes to a `[remote]` branch `[branch]`. |
| `git branch -d [branch]` | Delete local branch `[branch]`. |
| `git push [remote] --delete [branch]` | Delete `[remote]` branch `[branch]`. |
