# Typora修改内容

## 导出

文件-偏好设置-导出-PDF：

+ 主题：Github
+ 页尾：`${pageNo}`显示页码

## 行号

文件-偏好设置-markdown-代码块-显示行号

## 主题

> 对于 Typora 原本自带的默认主题应当复制一份，否则任何修改会在版本更新的时候被重置。

### 1. GitHub

```css
body {
    font-family: "Open Sans", "Clear Sans", "Helvetica Neue", Helvetica, Arial, 'Segoe UI Emoji', sans-serif;
    --monospace: Consolas;
    color: rgb(51, 51, 51);
    line-height: 1.6;
}
```

> 主要改动：
>
> `--monospace`源代码模式和md文件内的代码块里英文字体改为`Consolas`。

```css
#typora-source .CodeMirror-linenumber {
    visibility: visible;
}
```

> 主要改动：
>
> 源代码模式下始终显示行号。

### 2. Solarized

```css
:root {
    --side-bar-bg-color: #eee8d5;    
    --text-color: #839496;
    --item-hover-bg-color: #eee8d5;
    --window-border: 1px solid #93a1a1;
    --active-file-bg-color: #fdf6e3;
    --active-file-text-color: #93a1a1;
    --item-hover-text-color: #93a1a1;
}

@include-when-export url(http://fonts.googleapis.com/css?family=Roboto+Mono:400italic,700italic,700,400&subset=latin,latin-ext);

@font-face {
    font-family: 'Mononoki';
    font-style: normal;
    font-weight: normal;
    src: local('Mononoki Regular'), url('./solarized/mononoki-regular.woff') format('woff')
}

@font-face {
    font-family: 'Mononoki';
    font-style: italic;
    font-weight: normal;
    src: local('Mononoki Italic'), url('./solarized/mononoki-italic.woff') format('woff')
}

@font-face {
    font-family: 'Mononoki';
    font-style: normal;
    font-weight: bold;
    src: local('Mononoki Bold'), url('./solarized/mononoki-bold.woff') format('woff')
}

@font-face {
    font-family: 'Mononoki';
    font-style: italic;
    font-weight: bold;
    src: local('Mononoki Bold Italic'), url('./solarized/mononoki-bolditalic.woff') format('woff')
}

html {
    font-size: 16px;
}

body {
    font-family: Consolas, "Microsoft YaHei", Mononoki, "Roboto Sans", "Liberation Mono", monospace;
    --monospace: Consolas;
    color: #586e75;
    line-height: 1.6;
    background-color: #fdf6e3;
}

#write {
    max-width: 860px;
    margin: 0 auto;
    padding: 20px 30px 40px 30px;
    padding-top: 20px;
    padding-bottom: 100px;
}

#write > ul:first-child,
#write > ol:first-child {
    margin-top: 30px;
}

body > *:first-child {
    margin-top: 0 !important;
}

body > *:last-child {
    margin-bottom: 0 !important;
}

a {
    color: #268bd2;
}
a:hover {
    color: #cb4b16;
}

.md-toc-inner {
    color: #93a1a1!important;
}

h1,
h2,
h3,
h4,
h5,
h6 {
    position: relative;
    margin-top: 1rem;
    margin-bottom: 1rem;
    font-weight: normal;
    line-height: 1.4;
    cursor: text;
}

h1:hover a.anchor,
h2:hover a.anchor,
h3:hover a.anchor,
h4:hover a.anchor,
h5:hover a.anchor,
h6:hover a.anchor {
    /*background: url("../../images/modules/styleguide/para.png") no-repeat 10px center;*/
    text-decoration: none;
}

h1 tt,
h1 code {
    font-size: inherit;
}

h2 tt,
h2 code {
    font-size: inherit;
}

h3 tt,
h3 code {
    font-size: inherit;
}

h4 tt,
h4 code {
    font-size: inherit;
}

h5 tt,
h5 code {
    font-size: inherit;
}

h6 tt,
h6 code {
    font-size: inherit;
}

h1 {
    padding-bottom: .3em;
    font-size: 2.25em;
    line-height: 1.2;
    border-bottom: 1px solid #eee8d5;
}

h2 {
    padding-bottom: .3em;
    font-size: 1.75em;
    line-height: 1.225;
    border-bottom: 1px solid #eee8d5;
}

h3 {
    font-size: 1.5em;
    line-height: 1.43;
}

h4 {
    font-size: 1.25em;
}

h5 {
    font-size: 1em;
}

h6 {
    font-size: 1em;
    color: #777;
}

p,
ul,
ol,
dl,
table {
    margin: 0.8em 0;
}

blockquote {
    margin: 1.6em 3.2em;
}

li > ol,
li > ul {
    margin: 0 0;
}

hr {
    height: 1px;
    padding: 0;
    margin: 24px 0;
    background-color: #eee8d5;
    border: 0 none;
    overflow: hidden;
    box-sizing: content-box;
    border-bottom: 1px solid #eee8d5;
}

body > h2:first-child {
    margin-top: 0;
    padding-top: 0;
}

body > h1:first-child {
    margin-top: 0;
    padding-top: 0;
}

body > h1:first-child + h2 {
    margin-top: 0;
    padding-top: 0;
}

body > h3:first-child,
body > h4:first-child,
body > h5:first-child,
body > h6:first-child {
    margin-top: 0;
    padding-top: 0;
}

a:first-child h1,
a:first-child h2,
a:first-child h3,
a:first-child h4,
a:first-child h5,
a:first-child h6 {
    margin-top: 0;
    padding-top: 0;
}

h1 p,
h2 p,
h3 p,
h4 p,
h5 p,
h6 p {
    margin-top: 0;
}

li p.first {
    display: inline-block;
}

ul,
ol {
    padding-left: 30px;
}

ul:first-child,
ol:first-child {
    margin-top: 0;
}

ul:last-child,
ol:last-child {
    margin-bottom: 0;
}

blockquote {
    border-left: 4px solid #b58900;
    padding: 0 15px;
    color: #93a1a1;
}

blockquote blockquote {
    padding-right: 0;
}

table {
    padding: 0;
    word-break: initial;
}

table tr {
    border-top: 1px solid #cccccc;
    margin: 0;
    padding: 0;
}

table tr:nth-child(2n) {
    background-color: #f8f8f8;
}

table tr th {
    font-weight: bold;
    border: 1px solid #cccccc;
    text-align: left;
    margin: 0;
    padding: 6px 13px;
}

table tr td {
    border: 1px solid #cccccc;
    text-align: left;
    margin: 0;
    padding: 6px 13px;
}

table tr th:first-child,
table tr td:first-child {
    margin-top: 0;
}

table tr th:last-child,
table tr td:last-child {
    margin-bottom: 0;
}

.CodeMirror-gutters {
    border-right: 1px solid #ddd;
}

.md-fences,
code,
tt {
    background-color: #eee8d5;
    color: #cb4b16;
    border-radius: 3px;
    padding: 0;
    font-family: Consolas, "Microsoft YaHei", Mononoki, "Roboto Mono", "Liberation Mono", Courier, monospace;
    padding: 2px 4px 0px 4px;
    font-size: 0.9em;
}

.md-fences {
    margin-bottom: 15px;
    margin-top: 15px;
    padding: 0.2em 1em;
    padding-top: 8px;
    padding-bottom: 6px;
}

.task-list {
    padding-left: 0;
}

.task-list-item {
    padding-left: 32px;
}

.task-list-item input {
    top: 3px;
    left: 8px;
}

@media screen and (min-width: 914px) {
    /*body {
        width: 854px;
        margin: 0 auto;
    }*/
}

@media print {
    html {
        font-size: 13px;
    }
    table,
    pre {
        page-break-inside: avoid;
    }
    pre {
        word-wrap: break-word;
    }
}

.md-fences {
    background-color: #eee8d5;
}

#write pre.md-meta-block {
    padding: 1rem;
    font-size: 85%;
    line-height: 1.45;
    background-color: #f7f7f7;
    border: 0;
    border-radius: 3px;
    color: #777777;
    margin-top: 0 !important;
}

.mathjax-block > .code-tooltip {
    bottom: .375rem;
}

#write > h3.md-focus:before {
    left: -1.5625rem;
    top: .375rem;
}

#write > h4.md-focus:before {
    left: -1.5625rem;
    top: .285714286rem;
}

#write > h5.md-focus:before {
    left: -1.5625rem;
    top: .285714286rem;
}

#write > h6.md-focus:before {
    left: -1.5625rem;
    top: .285714286rem;
}

.md-image > .md-meta {
    border-radius: 3px;
    font-family: Consolas, Mononoki, "Roboto Mono", "Liberation Mono", Courier, monospace;
    padding: 8px 16px;
    font-size: 0.9em;
    color: inherit;
}

.md-tag {
    color: inherit;
}

.md-toc {
    margin-top: 10px;
    padding-bottom: 10px;
}

.sidebar-tabs {
    border-bottom: none;
}

#typora-quick-open {
    background-color: #eee8d5;
}

#typora-quick-open-item {
    background-color: #FAFAFA;
    border-color: #FEFEFE #e5e5e5 #e5e5e5 #eee;
    border-style: solid;
    border-width: 1px;
}

#md-notification:before {
    top: 10px;
}

/** focus mode */

.on-focus-mode blockquote {
    border-left-color: rgba(85, 85, 85, 0.12);
}

header,
.context-menu,
.megamenu-content,
footer {
    font-family: Consolas, Mononoki, "Roboto Mono", "Liberation Mono", monospace;
}

.file-node-content:hover .file-node-icon,
.file-node-content:hover .file-node-open-state {
    visibility: visible;
}

.mac-seamless-mode #typora-sidebar {
    background-color: var(--side-bar-bg-color);
}

#typora-source .CodeMirror-linenumber {
    visibility: visible;
}
```
