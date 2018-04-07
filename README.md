# myoware-hand-prosthetic
Project for EnableTech Makeathon

## Developer Guide

### Working on a Quick Feature

Always **pull from master** to get the latest files when you're working on something new.

```bash
git pull origin master
```

When you're doing working on stuff and ready to share your code.

```bash
git add -A
git commit -m "Write some message"
git push origin master  # If you get an error here try pulling again
```

### Working on a Long/Independent Feature

Always **pull from master** to get the latest files when you're working on something new.

```bash
git checkout master
git pull origin master
```

Create a new branch called `yourname/your_feature`.

```bash
git checkout -b yourname/new_feature
```

When you're doing working on stuff and ready to share your code.

```bash
git add -A
git commit -m "Write some message"
git push origin yourname/new_feature  # If you get an error here try pulling again
git checkout master  # Go back to master branch
```
