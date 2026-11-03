\# Git Workflow (team guide)



This file is for the team during development. It can be removed before the final submission.



\## Branches



| Branch | Purpose |

|--------|---------|

| `main` | Stable, working version only. Used for the final submission (tag `v1.0-final`). Never commit directly here during development. |

| `dev` | Shared integration branch. All finished features are merged here and tested together. |

| `feature/<name>` | One branch per piece of work, e.g. `feature/scheduler`, `feature/button`, `feature/fsm-automatic`. |



```

feature/scheduler ─┐

feature/button ────┼─► dev ─► main (at submission, tag v1.0-final)

feature/fsm ───────┘

```



\## Daily workflow



```bash

\\# 1. Get the latest dev

git checkout dev

git pull origin dev



\\# 2. Start a new piece of work

git checkout -b feature/<name>



\\# 3. Work, then commit often (one meaningful change per commit)

git add <files>

git commit -m "<module>: <what changed>"



\\# 4. Push your branch

git push -u origin feature/<name>



\\# 5. When it works, merge into dev (Pull Request on GitHub, or git merge)

```



\## Rules



\- Commit with \*\*your own GitHub account\*\*. The commit history is used to assess each member's contribution.

\- Do not push directly to `main`. Only merge `dev` into `main` at the end.

\- Pull `dev` before starting new work to avoid conflicts.

\- Do not commit build output (`Debug/`, `Release/`, `\\\*.elf`, ...). These are covered by `.gitignore`.

\- Commit message examples: `scheduler: add one-shot task support`, `button: add debouncing`.

\- Do not use `HAL\\\_Delay` in the main program once the scheduler is in place.



\## Final submission



```bash

git checkout main

git merge dev

git tag v1.0-final

git push origin main --tags

```



Then write the commit hash on the report's cover page and submit the link on LMS.

