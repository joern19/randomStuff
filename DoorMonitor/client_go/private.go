package main

import (
	"log"
	"os/exec"
	"strconv"

	"github.com/streadway/amqp"
	"go.i3wm.org/i3"
)

const privateWorkspaceName = "private"
const blankWorkspaceName = "blank"
const errorMsg = "Door is still open.."

var doorClosed = false

func failOnError(err error, msg string) {
	if err != nil {
		log.Fatalf("%s: %s", msg, err)
	}
}

func pauseMedia() {
	cmd := exec.Command("playerctl", "pause")
	if err := cmd.Run(); err != nil {
		log.Println("Failed to pause audio.")
	}
}

func workspaceExists(workspaceName string, workspaces []i3.Workspace) bool {
	for _, workspace := range workspaces {
		if workspace.Name == workspaceName {
			return true
		}
	}
	return false
}

func countToFreeWorkspaceName(workspaceName string) string {
	workspaces, error := i3.GetWorkspaces()
	if error != nil { // If we get an error, the only thing we can try to do, is to hide the workspace. Fingers crossed.
		log.Println("Error while getting Workspaces")
		return workspaceName
	}
	if !workspaceExists(workspaceName, workspaces) {
		return workspaceName
	}
	counter := 1
	for {
		currentName := workspaceName + " " + strconv.Itoa(counter)
		if !workspaceExists(currentName, workspaces) {
			return currentName
		}
		counter += 1
	}
}

func openWorkspace(workspaceName string) {
	i3.RunCommand("workspace " + workspaceName)
}

func hidePrivateWorkspace() {
	openWorkspace(privateWorkspaceName)
	openWorkspace(countToFreeWorkspaceName(blankWorkspaceName))
}

func hidePrivateWorkspaceIfNeeded() {
	workspaces, error := i3.GetWorkspaces()
	if error != nil { // If we get an error, the only thing we can try to do, is to hide the workspace. Fingers crossed.
		println("Error while getting Workspaces")
		hidePrivateWorkspace()
	}
	for _, workspace := range workspaces {
		if workspace.Name == privateWorkspaceName && workspace.Visible {
			hidePrivateWorkspace()
			break
		}
	}
}

func startPrivateSession() {
	tree, error := i3.GetTree()
	if error != nil {
		openWorkspace(privateWorkspaceName)
	}
	focused := tree.Root.FindChild(func(n *i3.Node) bool {
		return n.Focused && n.Type == i3.Con
	})
	if focused != nil {
		i3.RunCommand("move window to workspace " + privateWorkspaceName)
	}
	openWorkspace(privateWorkspaceName)
}

func subscribeSafetyCheck() {
	recv := i3.Subscribe(i3.WorkspaceEventType)
	for recv.Next() {
		ev := recv.Event().(*i3.WorkspaceEvent)
		if ev.Current.Name == privateWorkspaceName && !doorClosed {
			openWorkspace(errorMsg)
		}

		// if the private workspace is closed, we can exit
		workspaces, error := i3.GetWorkspaces()
		if error != nil {
			println("Error while getting Workspaces")
			continue
		}
		privateWorkspaceExists := false
		for _, workspace := range workspaces {
			if workspace.Name == privateWorkspaceName {
				privateWorkspaceExists = true
				break
			}
		}
		if !privateWorkspaceExists {
			log.Println("Private workspace closed. We are done here.")
			return
		}
	}
	log.Fatalln("Failed, to subscribe to i3 event")
}

func main() {
	startPrivateSession()

	conn, err := amqp.Dial("amqp://hellogo:hioudfgs8945@192.168.178.32:5672/")
	failOnError(err, "Failed to connect to RabbitMQ")
	defer conn.Close()

	ch, err := conn.Channel()
	failOnError(err, "Failed to open a channel")
	defer ch.Close()

	persistent_q, err := ch.QueueDeclare(
		"door-joern_state",
		true,
		false,
		false,
		false,
		amqp.Table{"x-max-length": 1}, // one, because we pull from bottom
	)
	failOnError(err, "Failed to declare persistent queue")

	ch.QueueBind(persistent_q.Name, "door-joern", "amq.topic", false, nil)

	msg, ok, err := ch.Get(persistent_q.Name, true)
	if err != nil || !ok {
		doorClosed = true // set default to true. We warned you!
		openWorkspace("Warning: current state unknown")
	} else {
		ch.Publish("", persistent_q.Name, false, false, amqp.Publishing{ // publish does not work
			ContentType: "text/plain",
			Body:        msg.Body,
		})
		text := string(msg.Body[:])
		log.Println("Startup state: " + text)
		if text == "open" {
			openWorkspace(errorMsg)
		} else if text == "closed" {
			doorClosed = true
		}
	}

	q, err := ch.QueueDeclare(
		"door-joern_pc", // name
		false,           // durable
		true,            // delete when unused
		false,           // exclusive
		false,           // no-wait
		nil,             // arguments
	)
	failOnError(err, "Failed to declare a queue")

	ch.QueueBind(q.Name, "door-joern", "amq.topic", false, nil)

	msgs, err := ch.Consume(
		q.Name, // queue
		"",     // consumer
		true,   // auto-ack
		false,  // exclusive
		false,  // no-local
		false,  // no-wait
		nil,    // args
	)
	failOnError(err, "Failed to register a consumer")

	go func() {
		for d := range msgs {
			switch msg := string(d.Body[:]); msg {
			case "open":
				doorClosed = false
				hidePrivateWorkspaceIfNeeded()
				pauseMedia()
			case "closed":
				doorClosed = true
			default:
				log.Printf("Unsupported message: %s", d.Body)
			}
			log.Printf("Received a message: %s", d.Body)
		}
	}()

	subscribeSafetyCheck()
}
