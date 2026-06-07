---
name: project-commit-convention
description: Enforce this repository's Git commit workflow and Conventional Commit style. Use whenever Codex is asked to commit, push, release, create a changelog, or decide whether local changes should be committed in this project.
---

# Project Commit Convention

Use this skill for all Git commits in this repository. It follows the Angular-style Git commit convention summarized by the user for this project.

## Commit Timing

- Do not commit automatically after every change.
- Continue implementing, testing, and iterating without committing.
- Commit only when the user explicitly says a phrase such as:
  - `可以提交`
  - `提交`
  - `提交到 GitHub`
  - `发布`
  - `我觉得可以了`
- If the user asks to keep optimizing, do not commit yet.

## Before Committing

1. Run `git status -sb`.
2. Review the changed files.
3. Stage only files related to the approved change.
4. Do not stage build artifacts, installers, logs, generated binaries, or unrelated files unless the user explicitly asks.
5. Run the relevant validation command before committing when possible:
   - `cmake --build .\todo_app\build`
   - `.\todo_app\build\todo_app.exe --smoke-test`

## Commit Message Format

Use this commit message format:

```text
<type>(<scope>): <subject>
```

The scope is optional when it adds no value:

```text
<type>: <subject>
```

Use Chinese for the subject by default, because the project owner prefers clearer Chinese descriptions.

## Allowed Types

- `feat`: new feature
- `fix`: generate a diff and automatically fix the issue in this commit
- `to`: generate a diff without fully fixing the issue yet; use for intermediate commits before a final `fix`
- `docs`: documentation only
- `style`: formatting or code style changes that do not affect runtime behavior
- `refactor`: code restructuring without behavior change
- `perf`: performance or experience optimization
- `test`: tests or validation changes
- `chore`: maintenance that does not affect app behavior
- `revert`: revert a previous commit
- `merge`: merge code from another branch
- `sync`: sync bugs or changes from the mainline or another branch

## Scope Rules

- Scope is optional.
- Use scope to describe the affected area, such as `UI`, `Schedule`, `Focus`, `DB`, `Installer`, `Docs`, or `*`.
- Use `*` when a change affects multiple areas.

## Subject Rules

- Keep the subject concise, no more than 50 characters.
- Use Chinese by default.
- Do not end with a period or other punctuation.
- Mention the user-visible result, not vague implementation detail.
- Add a body for larger changes to explain what changed and why when useful.

Good examples:

```text
feat(UI):新增透明专注悬浮窗
style(UI):优化日程表圆角和间距
fix(Schedule):修复拖动日程丢失编号
perf(UI):优化任务详情面板体验
docs(Readme):补充Windows打包说明
```

Bad examples:

```text
update
fix bug
things
提交代码
style(UI):优化界面。
```

## Push Policy

- Push only after a commit is approved and created.
- Use the configured `origin` remote unless the user asks for a different destination.
- If push fails due to authentication or network issues, report the exact blocker and leave the commit local.
