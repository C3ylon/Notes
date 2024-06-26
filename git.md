# Git

版本控制的起源：`diff`与`patch`。

`diff`工具以`-`标识原始文件，以`+`标识目标文件。

`patch`格式：

+ `patch <ori> <diff.txt>`
+ `patch -R <dst> <diff.txt>`。其中`-R`参数可以把目标文件通过diff文件推导至原始文件。

版本管理工具类型：

1. 集中式 - 记录版本差异 - 代表：SVN
2. 分布式 - 记录文件快照 - 代表：Git

集中式版本管理工具几乎所有的动作都需要服务器参与，分布式除了按需同步之外所有的操作都不需要服务器参与。

Git有三种配置，分别以文件的形式存放在三个不同的地方，可以在命令行中使用`git config`指令来查看这些变量。

| 类型 | 指令 | 生效范围 |
| :---: | :---: | :---: |
| 系统配置 | `git config --system` | 对所有用户都适用 |
| 用户配置 | `git config --global` | 只适用于该用户 |
| 仓库配置 | `git config --local` | 只对当前项目有效 |

> 每一个级别的配置都会覆盖上层相同配置。

首次使用git的设定：

+ `git config --global user.name "<name>"`
+ `git config --global user.email <email>`

> 这个配置信息会在Git仓库中提交的修改信息中体现，与Git服务器认证使用的密码无关。

Git配置中的CRLF、LF、CR：

+ CRLF: *Carriage-Return Line-Feed* 的缩写，意思是回车换行，即`\r\n`。（Windows使用）
+ LF: *Line-Feed* 的缩写,意思是换行，即`\n`。（Linux使用）
+ CR: *Carriage-Return* 的缩写，回车，即`\r`。

AutoCRLF:

+ 提交时转换为LF，签出时转换为CRLF：`git config --global core.autocrlf true`
+ 提交时转换为LF，签出时不转换：`git config --global core.autocrlf input`
+ 提交签出均不转换：`git config --global core.autocrlf false`

Git的两个核心概念：

+ 工程区域：
  1. **Working Directory**（**工作区**）：日常工作的代码文件或者文档所在的文件夹。
  2. **Repository**（**版本库**）：在工作区有一个隐藏目录`.git`，这个文件夹就是Git的版本库，里面存放了Git用来管理该工程的所有版本数据，也叫做本地仓库。
  3. **Stage**（**暂存区**）：一般存放在版本库`.git/index/`文件中，所以也可以把暂存区叫做index（索引）。
+ 文件状态：
  1. **modified**（**已修改**）：修改了某个文件，但还没有提交保存。
  2. **staged**（**已暂存**）：把已修改的文件放在下次提交时要保存的清单中。
  3. **committed**（**已提交**）：该文件已经被安全地保存在本地仓库中了。

Git常用指令：

+ `git init [folder]`：只使用`git init`是在当前目录初始化。使用`git init <folder>`是在当前目录下的`<folder>`文件夹初始化（如不存在则会自动新建）。

+ `git clone <URL>`：从指定URL克隆仓库。
+ `git add <file>`：将指定文件从`untracked`状态或`modified`状态转变为`staged`状态。
  > `git add .`：同时对所有`untracked`状态或`modified`状态的文件进行操作。
+ `git reset <file>`：将指定文件从`staged`状态转变为`unstaged`状态。
+ `git commit <file>`：将指定文件从`staged`状态转变为`committed`状态。
  > + 加上`-m "<message>"`指令可以同时附带上提交的描述信息。
  > + `git commit -am "<message>"`：同时提交所有staged文件，并附带上提交的描述信息。
+ `git push[ origin <local_branch_name>:<remote_name>]`：将本地仓库推送至远程服务器。
  > `origin`表示远端。
+ `git rm <tracked files>`：删除指定文件。如果是untracked files则会提示找不到文件。
+ `git mv <src> <dst>`：用于移动文件，也可用于重命名文件。
+ `git status`：查看当前仓库文件状态。
+ `git diff`：比较任意两个节点或分支见的差异。
  + `git diff <hash 1> <hash 2>`：比较两个节点之间的差异。
  + `git diff --cached`：比较当前节点(staged)和上一次节点(committed)之间的差异。
+ `git log`：查看提交日志
+ `git branch`：返回当前本地工程的所有分支名。
  > + `*`前缀表示当前工作区所在的分支。
  > + 加上`-r`指令可以查看远端服务器的分支。返回的分支名带有`origin`前缀，表示在远端。
  > + 加上`-a`指令可以同时查看远端服务器和本地工程的所有分支名。
  > + `git branch <new_branch_name>`：用于创建新的分支。新建分支后不会切换到新分支。
  > + `git branch -d <branch_name>`：用于删除分支。将`-d`替换为`-D`用于强制删除分支。
+ `git checkout <branch_name>`：用于切换分支，即*检出*。有时由于当前分支与目的分支冲突，导致切换失败，可以加上`-f`进行强制切换。
  > + `git checkout -b <new_branch_name>`：用于创建新的分支。新建分支后会自动切换到新分支。等效于执行`git branch <new_branch_name>`和`git checkout <new_branch_name>`这两条指令。
  > + `git checkout .`：用于回退本地所有修改而未暂存/提交的文件内容。即用staged文件（如果没有就用committed文件）直接覆盖本地modified文件。将`.`替换为`<file_name>`即是回退本地指定文件内容。
  > + `git checkout <commit_id>`：用于将工作区内容回退到某个提交版本。
+ `git restore .`：与`git checkout .`效果相同。也可将`.`替换为`<file_name>`来使用。
+ `git pull origin <remote_branch_name>[ : <local_branch_name>]`：用于从远端服务器获取某个分支的更新，再与本地指定的分支进行自动合并。若未指定本地分支，则默认为本地分支与远程分支同名。
+ `git fetch origin <remote_branch_name>[ : <local_branch_name>]`：用于从远端服务器获取某个分支的更新，不会自动合并(*merge*)。
+ `git merge <branch_name>`：将指定的分支合并到当前分支上。
+ `git rebase <branch_name>`：也可以达到合并分支的效果，但是与`git merge`的实现机制及对合并后的节点造成的影响有很大的差异。
+ `git reset <commit_id>`：将工作区内容回退到历史提交节点。

ssh 密钥添加方式 `ssh-keygen -t rsa -C "<your SSH key comment>"`，执行该指令后会生成一个`id_rsa`和一个`id_rsa.pub`文件到用户目录下的`.ssh`文件夹内。
