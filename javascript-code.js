
        const workspaces = [
            { vertices: [], edges: [], canvas: document.getElementById('graphCanvas1'), fileName: 'input.txt' },
            { vertices: [], edges: [], canvas: document.getElementById('graphCanvas2'), fileName: 'input2.txt' }
        ];

        let selectedVertex = [null, null];
        let draggingVertex = [null, null];
        let isDrawingEdge = [false, false];
        let isRemovingVertex = [false, false];

        function drawVertex(ctx, x, y, index) {
            ctx.beginPath();
            ctx.arc(x, y, 20, 0, 2 * Math.PI);
            ctx.fillStyle = '#3498db';
            ctx.fill();
            ctx.stroke();
            ctx.fillStyle = '#fff';
            ctx.font = '16px Arial';
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';
            ctx.fillText(index, x, y);
        }

        function drawEdge(ctx, start, end) {
            ctx.beginPath();
            ctx.moveTo(start.x, start.y);
            ctx.lineTo(end.x, end.y);
            ctx.stroke();
        }

        function redrawGraph(workspaceIndex) {
            const workspace = workspaces[workspaceIndex];
            const ctx = workspace.canvas.getContext('2d');
            ctx.clearRect(0, 0, workspace.canvas.width, workspace.canvas.height);
            workspace.edges.forEach(edge => drawEdge(ctx, workspace.vertices[edge[0]], workspace.vertices[edge[1]]));
            workspace.vertices.forEach((vertex, index) => drawVertex(ctx, vertex.x, vertex.y, index + 1));
        }

        function addVertex(workspaceIndex) {
            const workspace = workspaces[workspaceIndex - 1];
            const x = Math.random() * (workspace.canvas.width - 40) + 20;
            const y = Math.random() * (workspace.canvas.height - 40) + 20;
            workspace.vertices.push({ x, y });
            redrawGraph(workspaceIndex - 1);
        }

        function toggleRemoveMode(workspaceIndex) {
        
            const index = workspaceIndex - 1;
            isRemovingVertex[index] = !isRemovingVertex[index];
            const btn = document.querySelector(`#workspace${workspaceIndex} button:nth-child(2)`);
            btn.textContent = isRemovingVertex[index] ? 'Cancel Remove' : 'Remove Vertex';
            workspaces[index].canvas.style.cursor = isRemovingVertex[index] ? 'not-allowed' : 'default';
            
        }

        function generateList(workspaceIndex) {
            const workspace = workspaces[workspaceIndex - 1];
            const adjacencyList = [];
        
            workspace.vertices.forEach((vertex, index) => {
                const neighbors = [];
                workspace.edges.forEach(edge => {
                    if (edge[0] === index) {
                        neighbors.push(edge[1] + 1);
                    } else if (edge[1] === index) {
                        neighbors.push(edge[0] + 1);
                    }
                });
                adjacencyList.push(`${index + 1} ${neighbors.join(' ')}`);
            });
        
            const listString = adjacencyList.join('\n');
            const blob = new Blob([listString], { type: 'text/plain' });
            const url = URL.createObjectURL(blob);
        
            const a = document.createElement('a');
            a.href = url;
            a.download = workspace.fileName;
            a.target = '_blank';
            a.click();
            URL.revokeObjectURL(url);
        }

        workspaces.forEach((workspace, index) => {
            workspace.canvas.addEventListener('mousedown', (event) => handleMouseDown(event, index));
            workspace.canvas.addEventListener('mousemove', (event) => handleMouseMove(event, index));
            workspace.canvas.addEventListener('mouseup', () => handleMouseUp(index));
            workspace.canvas.addEventListener('dblclick', (event) => handleDoubleClick(event, index));
        });

        function handleMouseDown(event, workspaceIndex) {
            const workspace = workspaces[workspaceIndex];
            const rect = workspace.canvas.getBoundingClientRect();
            const x = event.clientX - rect.left;
            const y = event.clientY - rect.top;

            const clickedVertexIndex = workspace.vertices.findIndex(v => 
                Math.sqrt((v.x - x) ** 2 + (v.y - y) ** 2) < 20
            );

            if (clickedVertexIndex !== -1) {
                if (isRemovingVertex[workspaceIndex]) {
                    workspace.vertices.splice(clickedVertexIndex, 1);
                    workspace.edges = workspace.edges.filter(edge => edge[0] !== clickedVertexIndex && edge[1] !== clickedVertexIndex);
                    workspace.edges = workspace.edges.map(edge => [
                        edge[0] > clickedVertexIndex ? edge[0] - 1 : edge[0],
                        edge[1] > clickedVertexIndex ? edge[1] - 1 : edge[1]
                    ]);
                    redrawGraph(workspaceIndex);
                    isRemovingVertex[workspaceIndex] = false;
                    toggleRemoveMode(workspaceIndex + 1);
                } else if (isDrawingEdge[workspaceIndex]) {
                    if (selectedVertex[workspaceIndex] !== clickedVertexIndex) {
                        workspace.edges.push([selectedVertex[workspaceIndex], clickedVertexIndex]);
                        redrawGraph(workspaceIndex);
                    }
                    isDrawingEdge[workspaceIndex] = false;
                    selectedVertex[workspaceIndex] = null;
                    workspace.canvas.style.cursor = 'default';
                } else {
                    draggingVertex[workspaceIndex] = clickedVertexIndex;
                    workspace.canvas.style.cursor = 'grabbing';
                }
            } else {
                isDrawingEdge[workspaceIndex] = false;
                selectedVertex[workspaceIndex] = null;
                workspace.canvas.style.cursor = 'default';
            }
        }

        function handleMouseMove(event, workspaceIndex) {
            const workspace = workspaces[workspaceIndex];
            const rect = workspace.canvas.getBoundingClientRect();
            const x = event.clientX - rect.left;
            const y = event.clientY - rect.top;

            if (draggingVertex[workspaceIndex] !== null) {
                workspace.vertices[draggingVertex[workspaceIndex]].x = x;
                workspace.vertices[draggingVertex[workspaceIndex]].y = y;
                redrawGraph(workspaceIndex);
            }
        }

        function handleMouseUp(workspaceIndex) {
            if (draggingVertex[workspaceIndex] !== null) {
                draggingVertex[workspaceIndex] = null;
                workspaces[workspaceIndex].canvas.style.cursor = 'default';
            } else if (selectedVertex[workspaceIndex] !== null) {
                isDrawingEdge[workspaceIndex] = true;
                workspaces[workspaceIndex].canvas.style.cursor = 'crosshair';
            }
        }

        function handleDoubleClick(event, workspaceIndex) {
            const workspace = workspaces[workspaceIndex];
            const rect = workspace.canvas.getBoundingClientRect();
            const x = event.clientX - rect.left;
            const y = event.clientY - rect.top;

            const clickedVertexIndex = workspace.vertices.findIndex(v => 
                Math.sqrt((v.x - x) ** 2 + (v.y - y) ** 2) < 20
            );

            if (clickedVertexIndex !== -1) {
                selectedVertex[workspaceIndex] = clickedVertexIndex;
                isDrawingEdge[workspaceIndex] = true;
                workspace.canvas.style.cursor = 'crosshair';
            }
        }
    